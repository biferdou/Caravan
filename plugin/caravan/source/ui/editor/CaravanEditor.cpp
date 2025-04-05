#include "ui/editor/CaravanEditor.h"

namespace Caravan
{
    CaravanEditor::CaravanEditor(CaravanProcessor &p)
        : AudioProcessorEditor(&p), processor(p)
    {
        setSize(600, 400);

        // Initialize custom look and feel
        lookAndFeel = std::make_unique<CaravanLookAndFeel>();
        setLookAndFeel(lookAndFeel.get());

        // Configure sliders with labels
        configureSlider(dustDriveSlider, true);
        dustDriveSlider.setName("Dust Drive");
        configureLabel(dustDriveLabel, "Dust Drive");

        configureSlider(widthSlider);
        configureLabel(widthLabel, "Width");

        configureSlider(airSlider);
        configureLabel(airLabel, "Air");

        configureSlider(deEsserSlider);
        configureLabel(deEsserLabel, "De-Esser");

        // Configure Tune Mode toggle
        tuneModeButton.setButtonText("Tune Mode");
        tuneModeButton.setLookAndFeel(lookAndFeel.get());
        tuneModeButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);

        // Add components to editor
        addAndMakeVisible(dustDriveSlider);
        addAndMakeVisible(dustDriveLabel);
        addAndMakeVisible(tuneModeButton);
        addAndMakeVisible(widthSlider);
        addAndMakeVisible(widthLabel);
        addAndMakeVisible(airSlider);
        addAndMakeVisible(airLabel);
        addAndMakeVisible(deEsserSlider);
        addAndMakeVisible(deEsserLabel);

        // Create parameter attachments
        sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.apvts, "dustDrive", dustDriveSlider));
        sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.apvts, "width", widthSlider));
        sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.apvts, "air", airSlider));
        sliderAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            processor.apvts, "deEsser", deEsserSlider));

        tuneModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            processor.apvts, "tuneMode", tuneModeButton);

        // Configure preset box
        setupPresetBox();
        addAndMakeVisible(presetBox);
    }

    CaravanEditor::~CaravanEditor()
    {
        setLookAndFeel(nullptr);
    }

    void CaravanEditor::paint(juce::Graphics &g)
    {
        // Draw gradient background
        auto bounds = getLocalBounds();
        juce::ColourGradient gradient(
            juce::Colour(0xFF1A1A1A), // Dark gray
            0.0f, 0.0f,
            juce::Colour(0xFF080808), // Nearly black
            static_cast<float>(bounds.getWidth()),
            static_cast<float>(bounds.getHeight()),
            false);
        g.setGradientFill(gradient);
        g.fillAll();

        // Draw title
        g.setColour(juce::Colours::white);
        g.setFont(juce::FontOptions().withHeight(30.0f));
        g.drawText("Caravan", getLocalBounds().removeFromTop(40),
                   juce::Justification::centred, false);

        // Draw tagline
        g.setFont(juce::FontOptions().withHeight(15.0f));
        g.setColour(juce::Colour(0xFFFFA500));
        g.drawText("Desert Vocal Processor",
                   getLocalBounds().removeFromTop(60).removeFromBottom(20),
                   juce::Justification::centred, false);
    }

    void CaravanEditor::resized()
    {
        auto bounds = getLocalBounds().reduced(20);

        // Header section
        auto headerBounds = bounds.removeFromTop(60);
        presetBox.setBounds(headerBounds.removeFromRight(150).reduced(5));

        // Main layout
        auto mainArea = bounds.reduced(10);

        // Main Dust Drive knob centered
        auto centralArea = mainArea.removeFromTop(180);
        auto dustDriveArea = centralArea.withSizeKeepingCentre(150, 150);
        dustDriveLabel.setBounds(dustDriveArea.removeFromTop(30));
        dustDriveSlider.setBounds(dustDriveArea);

        // Bottom controls row
        auto controlsArea = mainArea;
        int controlWidth = controlsArea.getWidth() / 4;

        auto tuneModeArea = controlsArea.removeFromLeft(controlWidth).reduced(5);
        tuneModeButton.setBounds(tuneModeArea.removeFromTop(30));

        auto widthArea = controlsArea.removeFromLeft(controlWidth).reduced(5);
        widthLabel.setBounds(widthArea.removeFromTop(20));
        widthSlider.setBounds(widthArea);

        auto airArea = controlsArea.removeFromLeft(controlWidth).reduced(5);
        airLabel.setBounds(airArea.removeFromTop(20));
        airSlider.setBounds(airArea);

        auto deEsserArea = controlsArea.removeFromLeft(controlWidth).reduced(5);
        deEsserLabel.setBounds(deEsserArea.removeFromTop(20));
        deEsserSlider.setBounds(deEsserArea);
    }

    void CaravanEditor::configureSlider(juce::Slider &slider, bool isMainDrive)
    {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
        slider.setLookAndFeel(lookAndFeel.get());

        if (isMainDrive)
        {
            // Main Dust Drive knob styling
            slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFFFA500));
            slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFFA500));
        }
        else
        {
            // Secondary controls styling
            slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFE69500));
            slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xFFE69500));
        }

        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void CaravanEditor::configureLabel(juce::Label &label, const juce::String &text)
    {
        label.setText(text, juce::dontSendNotification);
        label.setFont(juce::FontOptions().withHeight(15.0f));
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colours::white);
    }

    void CaravanEditor::setupPresetBox()
    {
        // Set up styling for preset box
        presetBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF333333));
        presetBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
        presetBox.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xFFFFA500));

        // Add "Default" as first item
        presetBox.addItem("Default", 1);

        // Add desert-themed presets
        presetBox.addItem("Oasis", 2);
        presetBox.addItem("Sandstorm", 3);
        presetBox.addItem("Mirage", 4);
        presetBox.addItem("Dunes", 5);
        presetBox.addItem("Sahara", 6);
        presetBox.addItem("Sunset", 7);
        presetBox.addItem("Pristine", 8);
        presetBox.addItem("Nomad", 9);

        // Set up callback for preset changes
        presetBox.onChange = [this]
        { handlePresetChange(); };
    }

    void CaravanEditor::handlePresetChange()
    {
        // Apply selected preset (subtract 2 because preset box indexing starts at 1, and first item is Default)
        int presetIndex = presetBox.getSelectedId() - 2;
        if (presetIndex >= -1)
        {
            processor.applyPreset(presetIndex);
        }
    }

}