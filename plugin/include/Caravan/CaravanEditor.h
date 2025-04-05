#pragma once
#include "CaravanProcessor.h"

class CaravanLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CaravanLookAndFeel();
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider &slider) override;

    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

class CaravanEditor : public juce::AudioProcessorEditor
{
public:
    explicit CaravanEditor(CaravanProcessor &);
    ~CaravanEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    // UI Configuration helpers
    void configureSlider(juce::Slider &, bool isMainDrive = false);
    void configureLabel(juce::Label &, const juce::String &);
    void handlePresetChange();

    CaravanProcessor &processor;

    // Main UI Components
    juce::Slider dustDriveSlider; // Main central knob
    juce::ToggleButton tuneModeButton;
    juce::Slider widthSlider;
    juce::Slider airSlider;
    juce::Slider deEsserSlider;

    // Labels
    juce::Label dustDriveLabel;
    juce::Label tuneModeLabel;
    juce::Label widthLabel;
    juce::Label airLabel;
    juce::Label deEsserLabel;

    // Parameter attachments
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tuneModeAttachment;

    // Styling and presets
    std::unique_ptr<CaravanLookAndFeel> lookAndFeel;
    juce::ComboBox presetBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CaravanEditor)
};