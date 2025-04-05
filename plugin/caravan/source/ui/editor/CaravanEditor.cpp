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

        // Configure sliders with labels
        configureSlider(dustDriveSlider, true);
        dustDriveSlider.setName("Dust Drive");

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

        // Add value change listeners to update parameters
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

        // Configure preset box with desert names
        setupPresetBox();
        addAndMakeVisible(presetBox);

        // Create title and version labels with custom font
        titleLabel.setText("CARAVAN", juce::dontSendNotification);
        titleLabel.setFont(fontManager.getFont(48.0f));
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(titleLabel);

        // Apply custom font to labels
        dustDriveLabel.setFont(fontManager.getFont(18.0f));
        widthLabel.setFont(fontManager.getFont(18.0f));
        airLabel.setFont(fontManager.getFont(18.0f));
        deEsserLabel.setFont(fontManager.getFont(18.0f));

        // Set the custom font in the LookAndFeel
        lookAndFeel->setFont(fontManager.getFont(15.0f));

        // Initialize OpenGL context
        openGLContext = std::make_unique<CaravanOpenGLContext>();

        startTimer(50); // Start timer for animations
    }

    CaravanEditor::~CaravanEditor()
    {
        setLookAndFeel(nullptr);
    }

    void CaravanEditor::updateParameters()
    {
        repaint();
    }

    void CaravanEditor::paint(juce::Graphics &g)
    {
        // Draw background image from binary resources
        auto backgroundImage = juce::ImageCache::getFromMemory(
            BinaryData::background_png, BinaryData::background_pngSize);

        if (backgroundImage.isValid())
        {
            g.drawImageAt(backgroundImage, 0, 0);
        }
        else
        {
            // Fallback if image not found
            g.fillAll(juce::Colour(0xFF1A1A1A));
        }
    }

    void CaravanEditor::resized()
    {
        auto bounds = getLocalBounds().reduced(20);

        // Header section with title and version
        auto headerBounds = bounds.removeFromTop(80);           // Reduced from 100
        titleLabel.setBounds(headerBounds.removeFromTop(50));   // Reduced from 60
        versionLabel.setBounds(headerBounds.removeFromTop(25)); // Reduced from 30

        // Preset box on the right top corner
        presetBox.setBounds(bounds.getRight() - 180, 30, 180, 30);

        // Main area for controls
        auto mainArea = bounds.reduced(5);

        // Calculate dimensions for dust drive section - still larger but fits 800x600
        int dustDriveSize = 320; // Increased size but not double (fits better in 800x600)

        // Center the dust drive knob horizontally
        int horizontalCenter = mainArea.getCentreX();
        int topPosition = 100; // Position from top

        // Create a centered area for the Dust Drive knob
        auto dustDriveArea = juce::Rectangle<int>(
            horizontalCenter - dustDriveSize / 2,
            topPosition,
            dustDriveSize,
            dustDriveSize);

        // Position Dust Drive label above the knob
        dustDriveLabel.setBounds(dustDriveArea.getX(), dustDriveArea.getY() - 25, dustDriveSize, 25);

        // Position the main knob
        dustDriveSlider.setBounds(dustDriveArea);

        // Calculate the area for secondary controls - place them in a symmetrical row below
        const int bottomRowY = dustDriveArea.getBottom() + 20; // Gap after main knob
        const int smallKnobSize = 90;                          // Size for smaller knobs (reduced)
        const int spacing = 20;                                // Space between knobs (reduced)

        // Calculate total width needed for all 4 controls
        int totalControlsWidth = 4 * smallKnobSize + 3 * spacing;

        // Starting X position to center the row
        int startX = horizontalCenter - totalControlsWidth / 2;

        // Position Tune Mode button
        tuneModeButton.setBounds(startX, bottomRowY, smallKnobSize, 30);

        // Position Width control
        widthLabel.setBounds(startX + smallKnobSize + spacing, bottomRowY - 20, smallKnobSize, 20);
        widthSlider.setBounds(startX + smallKnobSize + spacing, bottomRowY, smallKnobSize, smallKnobSize);

        // Position Air control
        airLabel.setBounds(startX + 2 * (smallKnobSize + spacing), bottomRowY - 20, smallKnobSize, 20);
        airSlider.setBounds(startX + 2 * (smallKnobSize + spacing), bottomRowY, smallKnobSize, smallKnobSize);

        // Position De-Esser control
        deEsserLabel.setBounds(startX + 3 * (smallKnobSize + spacing), bottomRowY - 20, smallKnobSize, 20);
        deEsserSlider.setBounds(startX + 3 * (smallKnobSize + spacing), bottomRowY, smallKnobSize, smallKnobSize);
    }
    void CaravanEditor::configureSlider(juce::Slider &slider, bool isMainDrive)
    {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 20);
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

        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void CaravanEditor::configureLabel(juce::Label &label, const juce::String &text)
    {
        label.setText(text, juce::dontSendNotification);
        label.setFont(fontManager.getFont(18.0f));
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colours::white);
    }

    void CaravanEditor::setupPresetBox()
    {
        // Clear any existing items first
        presetBox.clear();

        // Set up styling
        presetBox.setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
        presetBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
        presetBox.setColour(juce::ComboBox::arrowColourId, juce::Colours::orange);

        // Add a default item
        presetBox.addItem("Select Preset", 1);

        // Add all the preset names
        presetBox.addItem("Oasis", 2);
        presetBox.addItem("Sandstorm", 3);
        presetBox.addItem("Mirage", 4);
        presetBox.addItem("Dunes", 5);
        presetBox.addItem("Sahara", 6);
        presetBox.addItem("Sunset", 7);
        presetBox.addItem("Pristine", 8);
        presetBox.addItem("Nomad", 9);

        // Set up the callback - make sure you're using a lambda that captures 'this'
        presetBox.onChange = [this]()
        { handlePresetChange(); };

        // Set to first item
        presetBox.setSelectedItemIndex(0);
    }

    void CaravanEditor::handlePresetChange()
    {
        // Get the selected index, accounting for the fact that indices are zero-based
        // but IDs start from 1
        int selectedId = presetBox.getSelectedId();

        // Only apply preset if an actual preset is selected (not the "Select Preset" option)
        if (selectedId > 1)
        {
            // Convert from combo box ID to preset index (subtract 2)
            int presetIndex = selectedId - 2;

            // Debug output
            juce::Logger::writeToLog("Applying preset: " + juce::String(presetIndex));

            // Apply the preset to the processor
            processor.applyPreset(presetIndex);

            // Force a UI update of all parameters
            updateAllParameters();
        }
    }

    // Add this method to update all UI elements when a preset is selected
    void CaravanEditor::updateAllParameters()
    {
        // The SliderAttachments should automatically update the sliders,
        // but we can force a refresh of other UI elements

        // Update toggle buttons
        bool tuneMode = processor.apvts.getRawParameterValue("tuneMode")->load() > 0.5f;
        tuneModeButton.setToggleState(tuneMode, juce::dontSendNotification);

        // Force a repaint of the UI
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