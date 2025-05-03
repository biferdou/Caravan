#pragma once
#include "core/CaravanProcessor.h"
#include "ui/components/CaravanLookAndFeel.h"
#include "ui/components/CaravanFontManager.h"
#include "ui/opengl/CaravanOpenGLContext.h"

namespace Caravan
{
    class CaravanEditor : public juce::AudioProcessorEditor,
                          private juce::Timer
    {
    public:
        explicit CaravanEditor(CaravanProcessor &);
        ~CaravanEditor() override;

        void paint(juce::Graphics &) override;
        void resized() override;

        void timerCallback() override;

        CaravanOpenGLContext *getOpenGLContext() { return openGLContext.get(); }

    private:
        void configureSlider(juce::Slider &, bool isMainDrive = false);
        void configureLabel(juce::Label &, const juce::String &);
        void setupPresetCarousel();
        void cyclePreset(int direction);
        void updateParameters();
        void updateAllParameters();
        void updateOpenGLParameters();
        void updateTextColors();

        CaravanProcessor &processor;
        CaravanFontManager fontManager;

        juce::Slider dustDriveSlider;
        juce::ToggleButton tuneModeButton;
        juce::Slider widthSlider;
        juce::Slider airSlider;
        juce::Slider deEsserSlider;

        juce::Label dustDriveLabel;
        juce::Label widthLabel;
        juce::Label airLabel;
        juce::Label deEsserLabel;
        juce::Label tuneModeLabel;

        juce::Label titleLabel;
        juce::Label versionLabel;

        std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tuneModeAttachment;

        juce::TextButton prevPresetButton;
        juce::TextButton nextPresetButton;
        juce::Label presetNameLabel;
        int currentPresetIndex = 0;

        std::unique_ptr<CaravanLookAndFeel> lookAndFeel;
        std::unique_ptr<CaravanOpenGLContext> openGLContext;

        // New: vector to store preset background images
        std::vector<juce::Image> presetBackgroundImages;

        // New: store preset text colors
        std::vector<juce::Colour> presetTextColors;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CaravanEditor)
    };
}