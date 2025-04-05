#include "AudioParameters.h"

namespace Caravan
{

    juce::AudioProcessorValueTreeState::ParameterLayout AudioParameters::createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        // Main "Dust Drive" parameter
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "dustDrive", "Dust Drive", 0.0f, 1.0f, 0.5f));

        // Tune Mode switch
        params.push_back(std::make_unique<juce::AudioParameterBool>(
            "tuneMode", "Tune Mode", false));

        // Width control
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "width", "Width", 0.0f, 1.0f, 0.5f));

        // Air control
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "air", "Air", 0.0f, 1.0f, 0.5f));

        // De-esser control
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "deEsser", "De-Esser", 0.0f, 1.0f, 0.3f));

        return {params.begin(), params.end()};
    }

    float AudioParameters::getDustDriveRange(float normalized)
    {
        // Convert 0-1 to the appropriate range for the DSP code
        // This helps make the parameter response more intuitive
        return normalized; // Or apply custom mapping if needed
    }

    float AudioParameters::getWidthRange(float normalized)
    {
        // Map width from 0-1 to a more subtle range (0.8-1.5)
        return 0.8f + normalized * 0.7f;
    }

    float AudioParameters::getAirRange(float normalized)
    {
        return normalized;
    }

    float AudioParameters::getDeEsserRange(float normalized)
    {
        return normalized;
    }

}