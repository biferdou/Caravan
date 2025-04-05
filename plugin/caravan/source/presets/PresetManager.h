#pragma once
#include <string>
#include <vector>
#include <memory>
#include <juce_audio_processors/juce_audio_processors.h>

namespace Caravan
{

    class PresetManager
    {
    public:
        PresetManager(juce::AudioProcessorValueTreeState &apvts);
        ~PresetManager() = default;

        void initializePresets();
        void applyPreset(int presetIndex);

        int getNumPresets() const { return static_cast<int>(desertPresets.size()); }
        std::string getPresetName(int index) const;

        struct DesertPreset
        {
            std::string name;
            float dustDrive;
            bool tuneMode;
            float width;
            float air;
            float deEsser;
        };

    private:
        std::vector<DesertPreset> desertPresets;
        juce::AudioProcessorValueTreeState &apvts;
    };

}