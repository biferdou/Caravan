#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#include "effects/DustDriveSaturator.h"
#include "effects/StereoWidthProcessor.h"
#include "effects/VocalEQ.h"
#include "presets/PresetManager.h"

namespace Caravan
{

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
        // DSP modules
        juce::dsp::Gain<float> inputGain;
        VocalEQ vocalEQ;
        DustDriveSaturator dustDrive;
        StereoWidthProcessor stereoWidth;
        juce::dsp::Gain<float> outputGain;

        // Preset manager
        std::unique_ptr<PresetManager> presetManager;

        // Parameter pointers
        std::atomic<float> *dustDriveParam = nullptr;
        std::atomic<float> *tuneModeParam = nullptr;
        std::atomic<float> *widthParam = nullptr;
        std::atomic<float> *airParam = nullptr;
        std::atomic<float> *deEsserParam = nullptr;

        // Utility methods
        void updateProcessorSettings();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CaravanProcessor)
    };

} // namespace Caravan