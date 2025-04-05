#pragma once
#include <juce_dsp/juce_dsp.h>

namespace Caravan
{

    class DustDriveSaturator
    {
    public:
        DustDriveSaturator() = default;
        ~DustDriveSaturator() = default;

        void prepare(const juce::dsp::ProcessSpec &spec);
        void reset();

        // Process single sample
        float processSample(float sample);

        // Process entire buffer
        void processBlock(juce::AudioBuffer<float> &buffer);

        // Parameter setters
        void setDriveAmount(float newDriveAmount);

        // Returns whether to use multi-stage processing
        bool shouldUseMultistage() const;

    private:
        float driveAmount = 0.5f;
        float sampleRate = 44100.0f;
    };

}