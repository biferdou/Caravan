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
        void handlePresetChange();
        void setupPresetBox();
        void updateParameters();
        void updateAllParameters(); // Added this method for preset handling
        void updateOpenGLParameters();

        CaravanProcessor &processor;
        CaravanFontManager fontManager;

        // Main UI Components
        juce::Slider dustDriveSlider; // Main central knob
        juce::ToggleButton tuneModeButton;
        juce::Slider widthSlider;
        juce::Slider airSlider;
        juce::Slider deEsserSlider;

        // Labels
        juce::Label dustDriveLabel;
        juce::Label widthLabel;
        juce::Label airLabel;
        juce::Label deEsserLabel;

        // Title and version labels
        juce::Label titleLabel;
        juce::Label versionLabel;

        // Parameter attachments
        std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> sliderAttachments;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tuneModeAttachment;

        // Styling and presets
        std::unique_ptr<CaravanLookAndFeel> lookAndFeel;
        juce::ComboBox presetBox;

        // OpenGL context for desert UI
        std::unique_ptr<CaravanOpenGLContext> openGLContext;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CaravanEditor)
    };

}