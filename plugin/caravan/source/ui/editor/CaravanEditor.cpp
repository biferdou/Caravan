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

        // Load all preset background images
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::oasis_png, BinaryData::oasis_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::sandstorm_png, BinaryData::sandstorm_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::mirage_png, BinaryData::mirage_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::dunes_png, BinaryData::dunes_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::sahara_png, BinaryData::sahara_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::sunset_png, BinaryData::sunset_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::pristine_png, BinaryData::pristine_pngSize));
        presetBackgroundImages.push_back(juce::ImageCache::getFromMemory(BinaryData::nomad_png, BinaryData::nomad_pngSize));

        // Define preset text colors
        presetTextColors.push_back(juce::Colours::white);          // Oasis
        presetTextColors.push_back(juce::Colours::grey);           // Sandstorm
        presetTextColors.push_back(juce::Colours::bisque);         // Mirage
        presetTextColors.push_back(juce::Colours::darkolivegreen); // Dunes
        presetTextColors.push_back(juce::Colours::black);          // Sahara
        presetTextColors.push_back(juce::Colours::goldenrod);      // Sunset
        presetTextColors.push_back(juce::Colours::darkred);        // Pristine
        presetTextColors.push_back(juce::Colours::blue);           // Nomad

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
        tuneModeButton.setColour(juce::ToggleButton::textColourId, juce::Colours::black);
        tuneModeButton.setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);

        configureLabel(tuneModeLabel, "TUNE MODE");
        configureLabel(dustDriveLabel, "Dust Drive");

        addAndMakeVisible(dustDriveSlider);
        addAndMakeVisible(tuneModeButton);
        addAndMakeVisible(tuneModeLabel);
        addAndMakeVisible(widthSlider);
        addAndMakeVisible(widthLabel);
        addAndMakeVisible(airSlider);
        addAndMakeVisible(airLabel);
        addAndMakeVisible(deEsserSlider);
        addAndMakeVisible(deEsserLabel);

        titleLabel.setText("CARAVAN", juce::dontSendNotification);
        titleLabel.setFont(fontManager.getFont(90.0f));
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(titleLabel);

        versionLabel.setText("alpha v1.0.1", juce::dontSendNotification);
        versionLabel.setFont(fontManager.getFont(30.0f));
        versionLabel.setJustificationType(juce::Justification::bottomLeft);
        versionLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(versionLabel);

        tuneModeLabel.setFont(fontManager.getFont(45.0f).boldened());
        tuneModeLabel.setJustificationType(juce::Justification::centred);

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

        // Apply initial text colors
        updateTextColors();
    }

    CaravanEditor::~CaravanEditor()
    {
        setLookAndFeel(nullptr);
    }

    void CaravanEditor::paint(juce::Graphics &g)
    {
        // Check if we have a valid preset background image
        if (!presetBackgroundImages.empty() && currentPresetIndex >= 0 &&
            currentPresetIndex < static_cast<int>(presetBackgroundImages.size()))
        {
            auto &backgroundImage = presetBackgroundImages[currentPresetIndex];
            if (backgroundImage.isValid())
            {
                g.drawImageAt(backgroundImage, 0, 0);
                return;
            }
        }

        // Fallback if no valid image
        g.fillAll(juce::Colour(0xFF1A1A1A));
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

        int radioSize = 30;
        int labelWidth = 150;
        int spacing = 100;

        int tuneModeX = dustDriveArea.getRight() + spacing;
        int tuneModeY = dustDriveArea.getCentreY() - radioSize / 2;

        // Position the radio button
        tuneModeButton.setBounds(
            tuneModeX,
            tuneModeY,
            radioSize,
            radioSize);

        // Position the label below the radio button, centered
        tuneModeLabel.setBounds(
            tuneModeX - labelWidth / 2 + radioSize / 2,
            tuneModeY + radioSize + 10,
            labelWidth,
            30);

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

        versionLabel.setBounds(10, getHeight() - 30, 150, 25);
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
        label.setFont(fontManager.getFont(28.0f));
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colours::black);
    }

    void CaravanEditor::setupPresetCarousel()
    {
        presetNameLabel.setText("Select Preset", juce::dontSendNotification);
        presetNameLabel.setFont(fontManager.getFont(28.0f).boldened());
        presetNameLabel.setJustificationType(juce::Justification::centred);
        presetNameLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(presetNameLabel);

        prevPresetButton.setButtonText("<");
        prevPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        prevPresetButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::orange.darker());
        prevPresetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
        prevPresetButton.onClick = [this]()
        { cyclePreset(-1); };
        addAndMakeVisible(prevPresetButton);

        nextPresetButton.setButtonText(">");
        nextPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
        nextPresetButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::orange.darker());
        nextPresetButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
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
        updateTextColors();
        repaint();
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

    void CaravanEditor::updateTextColors()
    {
        // Make sure we have valid preset text colors
        if (currentPresetIndex >= 0 && currentPresetIndex < static_cast<int>(presetTextColors.size()))
        {
            juce::Colour textColor = presetTextColors[currentPresetIndex];

            // Update all label text colors
            titleLabel.setColour(juce::Label::textColourId, textColor);
            versionLabel.setColour(juce::Label::textColourId, textColor);
            dustDriveLabel.setColour(juce::Label::textColourId, textColor);
            widthLabel.setColour(juce::Label::textColourId, textColor);
            airLabel.setColour(juce::Label::textColourId, textColor);
            deEsserLabel.setColour(juce::Label::textColourId, textColor);
            tuneModeLabel.setColour(juce::Label::textColourId, textColor);
            presetNameLabel.setColour(juce::Label::textColourId, textColor);

            // Update button text colors
            prevPresetButton.setColour(juce::TextButton::textColourOffId, textColor);
            nextPresetButton.setColour(juce::TextButton::textColourOffId, textColor);

            // Update toggle button colors
            tuneModeButton.setColour(juce::ToggleButton::textColourId, textColor);
        }
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