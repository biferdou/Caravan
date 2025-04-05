#include "CaravanEditor.h"
#include "../components/CaravanLookAndFeel.h"
#include "BinaryData.h"

namespace Caravan
{
    CaravanEditor::CaravanEditor(CaravanProcessor &p)
        : AudioProcessorEditor(&p), processor(p)
    {
        setSize(800, 600);

        lookAndFeel = std::make_unique<CaravanLookAndFeel>();
        setLookAndFeel(lookAndFeel.get());

        configureSlider(dustDriveSlider, true);
        dustDriveSlider.setName("Dust Drive");

        configureSlider(widthSlider);
        configureLabel(widthLabel, "Width");

        configureSlider(airSlider);
        configureLabel(airLabel, "Air");

        configureSlider(deEsserSlider);
        configureLabel(deEsserLabel, "De-Esser");

        tuneModeButton.setButtonText("");
        tuneModeButton.setLookAndFeel(lookAndFeel.get());
        tuneModeButton.setColour(juce::ToggleButton::textColourId, juce::Colours::white);

        configureLabel(tuneModeLabel, "Tune Mode");
        configureLabel(dustDriveLabel, "Dust Drive");

        addAndMakeVisible(dustDriveSlider);
        addAndMakeVisible(dustDriveLabel);
        addAndMakeVisible(tuneModeButton);
        addAndMakeVisible(tuneModeLabel);
        addAndMakeVisible(widthSlider);
        addAndMakeVisible(widthLabel);
        addAndMakeVisible(airSlider);
        addAndMakeVisible(airLabel);
        addAndMakeVisible(deEsserSlider);
        addAndMakeVisible(deEsserLabel);

        titleLabel.setText("CARAVAN", juce::dontSendNotification);
        titleLabel.setFont(fontManager.getFont(48.0f));
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(titleLabel);

        versionLabel.setText("alpha v1.0.1", juce::dontSendNotification);
        versionLabel.setFont(fontManager.getFont(14.0f));
        versionLabel.setJustificationType(juce::Justification::bottomLeft);
        versionLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(versionLabel);

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

        dustDriveSlider.onValueChange = [this]()
        { updateParameters(); };
        widthSlider.onValueChange = [this]()
        { updateParameters(); };
        airSlider.onValueChange = [this]()
        { updateParameters(); };
        deEsserSlider.onValueChange = [this]()
        { updateParameters(); };
        tuneModeButton.onClick = [this]()
        { updateParameters(); };

        setupPresetCarousel();
        openGLContext = std::make_unique<CaravanOpenGLContext>();
        startTimer(50);
    }

    CaravanEditor::~CaravanEditor()
    {
        setLookAndFeel(nullptr);
    }

    void CaravanEditor::paint(juce::Graphics &g)
    {
        auto backgroundImage = juce::ImageCache::getFromMemory(
            BinaryData::background_png, BinaryData::background_pngSize);

        if (backgroundImage.isValid())
        {
            g.drawImageAt(backgroundImage, 0, 0);
        }
        else
        {
            g.fillAll(juce::Colour(0xFF1A1A1A));
        }
    }

    void CaravanEditor::resized()
    {
        auto bounds = getLocalBounds().reduced(20);
        auto headerBounds = bounds.removeFromTop(80);
        titleLabel.setBounds(headerBounds.removeFromTop(50));

        auto mainArea = bounds;

        int dustDriveSize = 320;
        int horizontalCenter = mainArea.getCentreX();
        int topPosition = 100;

        auto dustDriveArea = juce::Rectangle<int>(
            horizontalCenter - dustDriveSize / 2,
            topPosition,
            dustDriveSize,
            dustDriveSize);

        dustDriveLabel.setBounds(dustDriveArea.getX(), dustDriveArea.getY() - 30, dustDriveSize, 25);
        dustDriveSlider.setBounds(dustDriveArea);

        int buttonWidth = 80;
        int buttonHeight = 30;
        int tuneModeX = getWidth() - buttonWidth - 40;

        tuneModeLabel.setBounds(
            tuneModeX,
            dustDriveArea.getCentreY() - buttonHeight / 2 - 25,
            buttonWidth,
            20);

        tuneModeButton.setBounds(
            tuneModeX,
            dustDriveArea.getCentreY() - buttonHeight / 2,
            buttonWidth,
            buttonHeight);

        int secondaryKnobSize = 80;
        int secondaryKnobY = dustDriveArea.getBottom() + 30;
        int secondaryKnobSpacing = 40;
        int totalWidth = 3 * secondaryKnobSize + 2 * secondaryKnobSpacing;
        int secondaryKnobStartX = horizontalCenter - totalWidth / 2;

        widthLabel.setBounds(
            secondaryKnobStartX,
            secondaryKnobY - 25,
            secondaryKnobSize,
            20);
        widthSlider.setBounds(
            secondaryKnobStartX,
            secondaryKnobY,
            secondaryKnobSize,
            secondaryKnobSize);

        airLabel.setBounds(
            secondaryKnobStartX + secondaryKnobSize + secondaryKnobSpacing,
            secondaryKnobY - 25,
            secondaryKnobSize,
            20);
        airSlider.setBounds(
            secondaryKnobStartX + secondaryKnobSize + secondaryKnobSpacing,
            secondaryKnobY,
            secondaryKnobSize,
            secondaryKnobSize);

        deEsserLabel.setBounds(
            secondaryKnobStartX + 2 * (secondaryKnobSize + secondaryKnobSpacing),
            secondaryKnobY - 25,
            secondaryKnobSize,
            20);
        deEsserSlider.setBounds(
            secondaryKnobStartX + 2 * (secondaryKnobSize + secondaryKnobSpacing),
            secondaryKnobY,
            secondaryKnobSize,
            secondaryKnobSize);

        int carouselHeight = 40;
        int carouselButtonWidth = 40;
        int carouselY = getHeight() - carouselHeight - 10;

        prevPresetButton.setBounds(
            horizontalCenter - 120,
            carouselY,
            carouselButtonWidth,
            carouselHeight);
        presetNameLabel.setBounds(
            horizontalCenter - 70,
            carouselY,
            140,
            carouselHeight);
        nextPresetButton.setBounds(
            horizontalCenter + 80,
            carouselY,
            carouselButtonWidth,
            carouselHeight);

        versionLabel.setBounds(20, getHeight() - 30, 150, 25);
    }

    void CaravanEditor::configureSlider(juce::Slider &slider, bool isMainDrive)
    {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setLookAndFeel(lookAndFeel.get());

        if (isMainDrive)
        {
            slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
            slider.setColour(juce::Slider::thumbColourId, juce::Colours::orange);
        }
        else
        {
            slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange.darker());
            slider.setColour(juce::Slider::thumbColourId, juce::Colours::orange.darker());
        }
    }

    void CaravanEditor::configureLabel(juce::Label &label, const juce::String &text)
    {
        label.setText(text, juce::dontSendNotification);
        label.setFont(fontManager.getFont(18.0f));
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colours::white);
    }

    void CaravanEditor::setupPresetCarousel()
    {
        presetNameLabel.setText("Select Preset", juce::dontSendNotification);
        presetNameLabel.setFont(fontManager.getFont(18.0f));
        presetNameLabel.setJustificationType(juce::Justification::centred);
        presetNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(presetNameLabel);

        prevPresetButton.setButtonText("<");
        prevPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        prevPresetButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::orange.darker());
        prevPresetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        prevPresetButton.onClick = [this]()
        { cyclePreset(-1); };
        addAndMakeVisible(prevPresetButton);

        nextPresetButton.setButtonText(">");
        nextPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        nextPresetButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::orange.darker());
        nextPresetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
        nextPresetButton.onClick = [this]()
        { cyclePreset(1); };
        addAndMakeVisible(nextPresetButton);
    }

    void CaravanEditor::cyclePreset(int direction)
    {
        const int numPresets = 8;
        currentPresetIndex = (currentPresetIndex + direction + numPresets) % numPresets;

        processor.applyPreset(currentPresetIndex);

        const std::string presetName = processor.getPresetName(currentPresetIndex);
        presetNameLabel.setText(presetName, juce::dontSendNotification);

        updateAllParameters();
    }

    void CaravanEditor::updateParameters()
    {
        repaint();
    }

    void CaravanEditor::updateAllParameters()
    {
        bool tuneMode = processor.apvts.getRawParameterValue("tuneMode")->load() > 0.5f;
        tuneModeButton.setToggleState(tuneMode, juce::dontSendNotification);
        repaint();
    }

    void CaravanEditor::timerCallback()
    {
        updateOpenGLParameters();
    }

    void CaravanEditor::updateOpenGLParameters()
    {
        if (openGLContext == nullptr)
            return;

        float dustDriveValue = processor.apvts.getRawParameterValue("dustDrive")->load();
        float widthValue = processor.apvts.getRawParameterValue("width")->load();
        float airValue = processor.apvts.getRawParameterValue("air")->load();

        openGLContext->updateShaderUniforms(dustDriveValue, widthValue, airValue);
    }
}