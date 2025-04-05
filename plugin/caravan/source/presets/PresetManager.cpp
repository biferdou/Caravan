#include "PresetManager.h"

namespace Caravan
{

    PresetManager::PresetManager(juce::AudioProcessorValueTreeState &apvtsRef)
        : apvts(apvtsRef)
    {
        initializePresets();
    }

    void PresetManager::initializePresets()
    {
        // Make sure this initialization is formatted correctly - the error was likely here
        desertPresets = {
            // Oasis - Characterized by moderate saturation, strong presence, and subtle width
            {"Oasis", 0.68f, true, 0.55f, 0.72f, 0.42f},

            // Sandstorm - More aggressive saturation, wide stereo image, less de-essing
            {"Sandstorm", 0.85f, true, 0.75f, 0.48f, 0.25f},

            // Mirage - Medium saturation, controlled width, balanced air
            {"Mirage", 0.62f, true, 0.58f, 0.65f, 0.55f},

            // Dunes - Heavy saturation, moderate width, emphasized air
            {"Dunes", 0.78f, true, 0.52f, 0.82f, 0.48f},

            // Sahara - Medium saturation, widest stereo, spacious air
            {"Sahara", 0.65f, true, 0.82f, 0.74f, 0.40f},

            // Sunset - Cleanest saturation, balanced width, strong air and presence
            {"Sunset", 0.45f, true, 0.60f, 0.78f, 0.62f},

            // Pristine - Minimal saturation, natural width, gentle presence enhancement
            {"Pristine", 0.25f, false, 0.40f, 0.35f, 0.70f},

            // Nomad - No saturation, minimal processing, just subtle enhancement
            {"Nomad", 0.10f, false, 0.30f, 0.25f, 0.80f}};
    }

    void PresetManager::applyPreset(int presetIndex)
    {
        if (presetIndex >= 0 && presetIndex < static_cast<int>(desertPresets.size()))
        {
            const auto &preset = desertPresets[presetIndex];

            // Apply preset values to parameters
            // Using setValueNotifyingHost ensures proper automation recording in the DAW
            apvts.getParameter("dustDrive")->setValueNotifyingHost(preset.dustDrive);
            apvts.getParameter("tuneMode")->setValueNotifyingHost(preset.tuneMode ? 1.0f : 0.0f);
            apvts.getParameter("width")->setValueNotifyingHost(preset.width);
            apvts.getParameter("air")->setValueNotifyingHost(preset.air);
            apvts.getParameter("deEsser")->setValueNotifyingHost(preset.deEsser);
        }
    }

    std::string PresetManager::getPresetName(int index) const
    {
        if (index >= 0 && index < static_cast<int>(desertPresets.size()))
        {
            return desertPresets[index].name;
        }
        return "Default";
    }

}