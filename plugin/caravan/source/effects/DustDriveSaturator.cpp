#include "DustDriveSaturator.h"

namespace Caravan
{

    void DustDriveSaturator::prepare(const juce::dsp::ProcessSpec &spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);
        reset();
    }

    void DustDriveSaturator::reset()
    {
        // Reset any internal state if needed
    }

    float DustDriveSaturator::processSample(float sample)
    {
        // Coefficient values tuned based on NAV vocal characteristics
        const float preSaturationGain = 1.0f + driveAmount * 3.0f; // Range: 1.0 to 4.0
        const float positiveShaping = 1.2f;                        // More aggressive on positive peaks (consonants)
        const float negativeShaping = 0.8f;                        // Less aggressive on negative peaks (keeps warmth)
        const float highFreqEmphasis = 0.3f * driveAmount;         // Subtle high frequency enhancement

        // Apply pre-gain (pushes signal into saturation)
        float shaped = sample * preSaturationGain;

        // Get the sign before applying distortion
        float sign = shaped > 0.f ? 1.f : -1.f;
        float absValue = std::abs(shaped);

        // Calculate drive intensity (increases with input level for dynamic response)
        float drive = 1.0f + driveAmount * 6.0f; // Range: 1.0 to 7.0

        // Apply asymmetric saturation based on signal polarity
        float saturated;
        if (sign > 0)
        {
            // Tanh provides a smoother saturation curve than the previous exp function
            saturated = std::tanh(drive * absValue * positiveShaping);

            // Add subtle high-frequency emphasis on positive peaks (for presence)
            if (absValue > 0.1f)
            {
                saturated += highFreqEmphasis * saturated * (1.0f - std::exp(-absValue * 5.0f));
            }
        }
        else
        {
            // Softer curve on negative peaks to maintain warmth
            saturated = std::tanh(drive * absValue * negativeShaping);
        }

        saturated *= sign;

        // Apply drive-dependent limiting to prevent harsh clipping
        if (std::abs(saturated) > 0.95f)
        {
            saturated = sign * (0.95f + 0.05f * std::tanh((std::abs(saturated) - 0.95f) * 10.0f));
        }

        // Blend dry/wet based on drive amount for smooth control
        float wetMix = 0.4f + driveAmount * 0.6f; // Range: 0.4 to 1.0
        return sample * (1.0f - wetMix) + saturated * wetMix;
    }

    void DustDriveSaturator::processBlock(juce::AudioBuffer<float> &buffer)
    {
        // Higher drive values use multi-stage saturation for more extreme effect
        bool useMultistage = shouldUseMultistage();

        // Process each channel
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            float *channelData = buffer.getWritePointer(channel);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                // For higher drive values, use multi-stage saturation
                if (useMultistage)
                {
                    // First stage with moderate drive
                    float firstStage = processSample(channelData[sample]);

                    // Second stage with more drive for extreme settings
                    channelData[sample] = processSample(firstStage);
                }
                else
                {
                    // Single stage for cleaner settings
                    channelData[sample] = processSample(channelData[sample]);
                }
            }
        }
    }

    void DustDriveSaturator::setDriveAmount(float newDriveAmount)
    {
        driveAmount = newDriveAmount;
    }

    bool DustDriveSaturator::shouldUseMultistage() const
    {
        return driveAmount > 0.7f;
    }

}