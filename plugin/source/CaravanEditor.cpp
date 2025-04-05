#include "Caravan/CaravanEditor.h"

//==============================================================================
// CaravanLookAndFeel Implementation
CaravanLookAndFeel::CaravanLookAndFeel()
{
    // Set modern, hip-hop inspired color scheme
    setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFFA500));               // Orange
    setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFFFA500));    // Orange
    setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF333333)); // Dark gray
    setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    // Toggle button colors
    setColour(juce::ToggleButton::tickColourId, juce::Colour(0xFFFFA500));
    setColour(juce::ToggleButton::tickDisabledColourId, juce::Colour(0x80FFA500));
}

void CaravanLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                                          float sliderPosProportional, float rotaryStartAngle,
                                          float rotaryEndAngle, juce::Slider &slider)
{
    // Convert integer parameters to float at the start
    auto bounds = juce::Rectangle<float>(static_cast<float>(x),
                                         static_cast<float>(y),
                                         static_cast<float>(width),
                                         static_cast<float>(height));

    // Check if this is the main Dust Drive knob
    bool isMainKnob = slider.getName() == "Dust Drive";
    float radiusFactor = isMainKnob ? 0.75f : 0.4f;

    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * radiusFactor;
    auto toAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto center = bounds.getCentre();

    // Draw background
    if (isMainKnob)
    {
        // Create a glow effect for the main knob
        auto gradientBounds = juce::Rectangle<float>(center.getX() - radius * 1.2f,
                                                     center.getY() - radius * 1.2f,
                                                     radius * 2.4f, radius * 2.4f);

        juce::ColourGradient gradient(juce::Colour(0x40FFA500), center.getX(), center.getY(),
                                      juce::Colours::transparentBlack,
                                      center.getX() + radius * 1.2f, center.getY() + radius * 1.2f,
                                      true);
        g.setGradientFill(gradient);
        g.fillEllipse(gradientBounds);
    }

    // Draw the outer circle - using explicit casts to avoid float-to-int warnings
    g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
    g.drawEllipse(center.getX() - radius, center.getY() - radius,
                  radius * 2.0f, radius * 2.0f, 2.0f);

    // Draw the filled portion
    g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
    juce::Path filledArc;
    filledArc.addArc(center.getX() - radius, center.getY() - radius,
                     radius * 2.0f, radius * 2.0f,
                     rotaryStartAngle, toAngle, true);
    g.strokePath(filledArc, juce::PathStrokeType(isMainKnob ? 4.0f : 3.0f));

    // Draw pointer
    juce::Path pointer;
    auto pointerLength = radius * 0.8f;
    auto pointerThickness = isMainKnob ? 4.0f : 3.0f;

    pointer.addRectangle(-pointerThickness * 0.5f, -radius,
                         pointerThickness, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(toAngle)
                               .translated(center.getX(), center.getY()));
    g.setColour(slider.findColour(juce::Slider::thumbColourId));
    g.fillPath(pointer);

    // For main Dust Drive knob, add text overlay displaying percentage
    if (isMainKnob)
    {
        int percentage = static_cast<int>(sliderPosProportional * 100.0f);
        g.setFont(juce::FontOptions().withHeight(radius * 0.3f));
        g.setColour(juce::Colours::white);
        // Use explicit static_cast to int to avoid warnings
        g.drawText(juce::String(percentage) + "%",
                   static_cast<int>(center.getX() - radius),
                   static_cast<int>(center.getY() - radius * 0.2f),
                   static_cast<int>(radius * 2.0f),
                   static_cast<int>(radius * 0.4f),
                   juce::Justification::centred, false);
    }
}

void CaravanLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                                          bool shouldDrawButtonAsHighlighted,
                                          bool shouldDrawButtonAsDown)
{
    // Unused parameters are intentionally ignored
    juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto fontSize = juce::jmin(15.0f, (float)button.getHeight() * 0.6f);
    auto tickWidth = fontSize * 1.1f;

    // Draw box
    juce::Rectangle<float> tickBounds(4.0f, (button.getHeight() - tickWidth) * 0.5f,
                                      tickWidth, tickWidth);

    g.setColour(button.findColour(juce::ToggleButton::tickDisabledColourId)
                    .withAlpha(button.isEnabled() ? 1.0f : 0.5f));
    g.drawRoundedRectangle(tickBounds, 2.0f, 1.0f);

    // Draw tick if toggled on
    if (button.getToggleState())
    {
        g.setColour(button.findColour(juce::ToggleButton::tickColourId)
                        .withAlpha(button.isEnabled() ? 1.0f : 0.5f));

        auto tick = tickBounds.reduced(2.0f);
        g.fillRoundedRectangle(tick, 1.0f);

        // Draw "ON" text - using explicit casts to int
        g.setFont(juce::FontOptions().withHeight(fontSize));
        g.drawText("ON",
                   static_cast<int>(tickBounds.getRight() + 5.0f),
                   static_cast<int>(tickBounds.getY()),
                   static_cast<int>(button.getWidth() - tickBounds.getWidth() - 5.0f),
                   static_cast<int>(tickBounds.getHeight()),
                   juce::Justification::centredLeft, true);
    }
    else
    {
        // Draw "OFF" text when untoggled - using explicit casts to int
        g.setColour(juce::Colours::white.withAlpha(0.8f));
        g.setFont(juce::FontOptions().withHeight(fontSize));
        g.drawText("OFF",
                   static_cast<int>(tickBounds.getRight() + 5.0f),
                   static_cast<int>(tickBounds.getY()),
                   static_cast<int>(button.getWidth() - tickBounds.getWidth() - 5.0f),
                   static_cast<int>(tickBounds.getHeight()),
                   juce::Justification::centredLeft, true);
    }
}

//==============================================================================
// CaravanEditor Implementation
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
    addAndMakeVisible(presetBox);
    presetBox.addItem("Default", 1);
    presetBox.addItem("NAV", 2);
    presetBox.addItem("Carti", 3);
    presetBox.addItem("Travis", 4);
    presetBox.addItem("Future", 5);
    presetBox.addItem("Don Toliver", 6);
    presetBox.addItem("The Weeknd", 7);
    presetBox.addItem("Clean", 8);
    presetBox.addItem("Raw", 9);
    presetBox.onChange = [this]
    { handlePresetChange(); };
    presetBox.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xFF333333));
    presetBox.setColour(juce::ComboBox::textColourId, juce::Colours::white);
    presetBox.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xFFFFA500));
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
    g.drawText("Modern Vocal Processor",
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
    // Using FontOptions correctly to avoid warning
    label.setFont(juce::FontOptions().withHeight(15.0f));
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
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