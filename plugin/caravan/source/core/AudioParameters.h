#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace Caravan
{

    class AudioParameters
    {
    public:
        // Creates the APVTS parameter layout
        static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

        // Helper functions to retrieve parameter ranges
        static float getDustDriveRange(float normalized);
        static float getWidthRange(float normalized);
        static float getAirRange(float normalized);
        static float getDeEsserRange(float normalized);
    };

}