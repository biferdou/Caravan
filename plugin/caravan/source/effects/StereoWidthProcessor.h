#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace Caravan
{

    class StereoWidthProcessor
    {
    public:
        StereoWidthProcessor() = default;
        ~StereoWidthProcessor() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);
        void processBlock(juce::AudioBuffer<float> &buffer, float widthAmount);

    private:
        float sampleRate = 44100.0f;
    };

}