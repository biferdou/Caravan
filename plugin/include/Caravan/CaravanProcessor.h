#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class CaravanProcessor : public juce::AudioProcessor
{
public:
    CaravanProcessor();
    ~CaravanProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Caravan"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String &) override {}

    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    // Public method to apply presets from the editor
    void applyPreset(int presetIndex);

private:
    // DSP modules for vocal processing chain
    juce::dsp::Gain<float> inputGain;

    // Pre EQ (tuning mode)
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                   juce::dsp::IIR::Coefficients<float>>
        preEq;

    // De-esser (simplified as a basic filter for now)
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                   juce::dsp::IIR::Coefficients<float>>
        deEsser;

    // Custom waveshaper for Dust Drive saturation
    class DustDriveSaturator
    {
    public:
        void prepare(const juce::dsp::ProcessSpec &spec);
        void reset();
        float processSample(float sample);
        void setDriveAmount(float newDriveAmount);

    private:
        float driveAmount = 0.5f;
        float sampleRate = 44100.0f;
    };

    DustDriveSaturator dustDrive;

    // Width enhancement
    juce::dsp::Panner<float> stereoWidth;

    // Air/exciter (high shelf EQ)
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                   juce::dsp::IIR::Coefficients<float>>
        airExciter;

    // Output gain
    juce::dsp::Gain<float> outputGain;

    // Parameter pointers
    std::atomic<float> *dustDriveParam = nullptr;
    std::atomic<float> *tuneModeParam = nullptr;
    std::atomic<float> *widthParam = nullptr;
    std::atomic<float> *airParam = nullptr;
    std::atomic<float> *deEsserParam = nullptr;

    // Utility methods
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void updateProcessorSettings();
    void processWithDustDrive(juce::AudioBuffer<float> &buffer);
    void processWidth(juce::AudioBuffer<float> &buffer, float widthAmount);

    // preset system
    struct DustVoxPreset
    {
        std::string name;
        float dustDrive;
        bool tuneMode;
        float width;
        float air;
        float deEsser;
    };

    std::vector<DustVoxPreset> artistPresets;
    void initializePresets();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CaravanProcessor)
};