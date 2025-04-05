#pragma once
#include <juce_dsp/juce_dsp.h>

namespace Caravan
{

    class VocalEQ
    {
    public:
        VocalEQ();
        ~VocalEQ() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);
        void reset();

        // Update the processor settings based on parameters
        void updateSettings(bool tuneMode, float airValue, float deEsserValue);

        // Process the audio buffer
        void processBlock(juce::AudioBuffer<float> &buffer);

    private:
        // Pre EQ (tuning mode)
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                       juce::dsp::IIR::Coefficients<float>>
            preEq;

        // De-esser
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                       juce::dsp::IIR::Coefficients<float>>
            deEsser;

        // Air/exciter (high shelf EQ)
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                       juce::dsp::IIR::Coefficients<float>>
            airExciter;

        float sampleRate = 44100.0f;
    };

}