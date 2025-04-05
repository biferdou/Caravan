#include "StereoWidthProcessor.h"

namespace Caravan
{

    void StereoWidthProcessor::prepare(const juce::dsp::ProcessSpec &spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);
    }

    void StereoWidthProcessor::processBlock(juce::AudioBuffer<float> &buffer, float widthAmount)
    {
        if (buffer.getNumChannels() < 2)
            return;

        const float *leftIn = buffer.getReadPointer(0);
        const float *rightIn = buffer.getReadPointer(1);
        float *leftOut = buffer.getWritePointer(0);
        float *rightOut = buffer.getWritePointer(1);

        // Apply the width parameter using mid-side processing with frequency-dependent enhancement
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            // Extract mid and side signals
            float mid = (leftIn[i] + rightIn[i]) * 0.5f;
            float side = (leftIn[i] - rightIn[i]) * 0.5f;

            // Apply width enhancement
            // Map width from 0-1 to a more subtle range (0.8-1.5)
            // This gives more precise control in the useful range
            float sideGain = 0.8f + widthAmount * 0.7f;

            // Apply saturation to side signal for a more cohesive stereo image
            // This prevents the width effect from sounding artificial
            side = std::tanh(side * sideGain) / sideGain;

            // Convert back to left/right
            leftOut[i] = mid + side;
            rightOut[i] = mid - side;

            // Apply subtle compensation to maintain consistent volume across width settings
            float compensationGain = 1.0f - (widthAmount * 0.1f);
            leftOut[i] *= compensationGain;
            rightOut[i] *= compensationGain;
        }
    }

}