#include "CaravanLookAndFeel.h"
#include <cmath>
#include "BinaryData.h"

namespace Caravan
{

    CaravanLookAndFeel::CaravanLookAndFeel()
    {
        // Initialize colors
        sandColor = juce::Colours::sandybrown;
        duneColor = juce::Colours::saddlebrown;
        skyColor = juce::Colours::skyblue;
        sunColor = juce::Colours::orange;
        textColor = juce::Colours::white;
        shadowColor = juce::Colours::black.withAlpha(0.5f);

        // Initialize default font
        customFont = juce::Font(juce::FontOptions().withHeight(15.0f));

        // Initialize colors
        setColour(juce::Slider::thumbColourId, juce::Colours::orange);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

        // Toggle button colors
        setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
        setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::orange.withAlpha(0.5f));
        setColour(juce::ToggleButton::textColourId, juce::Colours::white);

        // Combo box colors
        setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
        setColour(juce::ComboBox::textColourId, juce::Colours::white);
        setColour(juce::ComboBox::arrowColourId, juce::Colours::orange);
        setColour(juce::ComboBox::buttonColourId, juce::Colours::darkgrey.darker());
        setColour(juce::ComboBox::outlineColourId, juce::Colours::darkgrey.darker());

        // Label colors
        setColour(juce::Label::textColourId, juce::Colours::white);
        setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);

        // Load the compass image once at initialization
        compassImage = juce::ImageCache::getFromMemory(
            BinaryData::compass_png, BinaryData::compass_pngSize);
    }

    CaravanLookAndFeel::~CaravanLookAndFeel()
    {
    }

    void CaravanLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                                              float sliderPosProportional, float rotaryStartAngle,
                                              float rotaryEndAngle, juce::Slider &slider)
    {
        // Special handling for Dust Drive main knob
        if (slider.getName() == "Dust Drive" && compassImage.isValid())
        {
            auto bounds = juce::Rectangle<int>(x, y, width, height);
            auto center = bounds.getCentre().toFloat();
            float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
            float imageSize = juce::jmin(width, height) * 0.98f;
            float originalWidth = static_cast<float>(compassImage.getWidth());
            float originalHeight = static_cast<float>(compassImage.getHeight());
            float scale = imageSize / juce::jmax(originalWidth, originalHeight);

            // Create transformation to rotate image around center
            juce::AffineTransform transform;
            transform = juce::AffineTransform::translation(-originalWidth / 2.0f, -originalHeight / 2.0f)
                            .rotated(angle)
                            .scaled(scale, scale)
                            .translated(center.x, center.y);

            g.drawImageTransformed(compassImage, transform);
            return;
        }

        // Standard knob drawing for secondary controls
        float knobSize = slider.getName() == "Dust Drive" ? 1.0f : 0.75f;
        auto bounds = juce::Rectangle<float>(static_cast<float>(x),
                                             static_cast<float>(y),
                                             static_cast<float>(width),
                                             static_cast<float>(height));

        // Resize for secondary knobs
        if (slider.getName() != "Dust Drive")
        {
            auto center = bounds.getCentre();
            float newSize = juce::jmin(bounds.getWidth(), bounds.getHeight()) * knobSize;
            bounds = juce::Rectangle<float>(center.x - newSize / 2.0f,
                                            center.y - newSize / 2.0f,
                                            newSize, newSize);
        }

        // Draw knob base
        float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.4f;
        auto center = bounds.getCentre();

        g.setColour(juce::Colours::darkgrey);
        g.fillEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

        g.setColour(juce::Colours::grey);
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        // Draw value arc
        float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(slider.getName() == "Dust Drive" ? juce::Colours::orange : juce::Colours::orange.darker());
        juce::Path arc;
        arc.addArc(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f,
                   rotaryStartAngle, angle, true);
        g.strokePath(arc, juce::PathStrokeType(3.0f));

        // Draw pointer
        juce::Path pointer;
        pointer.addRectangle(-1.5f, -radius, 3.0f, radius * 0.7f);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(center.x, center.y));
        g.setColour(juce::Colours::white);
        g.fillPath(pointer);
    }
    void CaravanLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
    {
        juce::ignoreUnused(shouldDrawButtonAsDown);

        bool isOn = button.getToggleState();
        bool isEnabled = button.isEnabled();
        float alpha = isEnabled ? 1.0f : 0.5f;

        auto fontSize = juce::jmin(15.0f, static_cast<float>(button.getHeight()) * 0.6f);
        auto tickWidth = fontSize * 1.1f;

        juce::Rectangle<float> tickBounds(4.0f, (static_cast<float>(button.getHeight()) - tickWidth) * 0.5f,
                                          tickWidth, tickWidth);

        g.setColour(juce::Colours::darkgrey.withAlpha(alpha));
        g.fillRoundedRectangle(tickBounds, 2.0f);

        g.setColour((shouldDrawButtonAsHighlighted ? juce::Colours::orange : juce::Colours::darkgrey.darker()).withAlpha(alpha));
        g.drawRoundedRectangle(tickBounds, 2.0f, 1.5f);

        if (isOn)
        {
            g.setColour(juce::Colours::orange.withAlpha(alpha));
            auto tick = tickBounds.reduced(3.0f);
            g.fillEllipse(tick);

            g.setFont(customFont.withHeight(fontSize));
            g.setColour(juce::Colours::white.withAlpha(alpha));
            g.drawText("ON",
                       static_cast<int>(tickBounds.getRight() + 5.0f),
                       static_cast<int>(tickBounds.getY()),
                       static_cast<int>(button.getWidth() - tickBounds.getWidth() - 5.0f),
                       static_cast<int>(tickBounds.getHeight()),
                       juce::Justification::centredLeft, true);
        }
        else
        {
            g.setFont(customFont.withHeight(fontSize));
            g.setColour(juce::Colours::white.withAlpha(0.8f * alpha));
            g.drawText("OFF",
                       static_cast<int>(tickBounds.getRight() + 5.0f),
                       static_cast<int>(tickBounds.getY()),
                       static_cast<int>(button.getWidth() - tickBounds.getWidth() - 5.0f),
                       static_cast<int>(tickBounds.getHeight()),
                       juce::Justification::centredLeft, true);
        }
    }

    void CaravanLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                                          int buttonX, int buttonY, int buttonW, int buttonH,
                                          juce::ComboBox &box)
    {
        juce::Rectangle<int> boxBounds(0, 0, width, height);
        g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
        g.fillRoundedRectangle(boxBounds.toFloat(), 3.0f);

        g.setColour(box.findColour(juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), 3.0f, 1.0f);

        juce::Rectangle<int> arrowBounds(buttonX, buttonY, buttonW, buttonH);

        juce::Path arrow;
        float arrowSize = static_cast<float>(juce::jmin(buttonW, buttonH)) * 0.3f;
        float x = static_cast<float>(buttonX) + static_cast<float>(buttonW) * 0.5f;
        float y = static_cast<float>(buttonY) + static_cast<float>(buttonH) * 0.5f;

        if (isButtonDown)
        {
            arrow.addTriangle(x - arrowSize, y, x + arrowSize, y, x, y - arrowSize);
        }
        else
        {
            arrow.addTriangle(x - arrowSize, y - arrowSize, x + arrowSize, y - arrowSize, x, y);
        }

        g.setColour(box.findColour(juce::ComboBox::arrowColourId));
        g.fillPath(arrow);
    }

    void CaravanLookAndFeel::positionComboBoxText(juce::ComboBox &box, juce::Label &label)
    {
        label.setBounds(10, 1, box.getWidth() - box.getHeight() - 15, box.getHeight() - 2);
        label.setFont(customFont.withHeight(static_cast<float>(box.getHeight()) * 0.6f));
    }

    void CaravanLookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label)
    {
        g.fillAll(label.findColour(juce::Label::backgroundColourId));

        if (!label.isBeingEdited())
        {
            auto alpha = label.isEnabled() ? 1.0f : 0.5f;
            const juce::Font font(label.getFont());

            g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
            g.setFont(font);
            g.drawText(label.getText(), label.getLocalBounds(),
                       label.getJustificationType(), true);
        }
        else if (label.isEnabled())
        {
            g.setColour(label.findColour(juce::Label::outlineColourId));
            g.drawRect(label.getLocalBounds());
        }
    }

    void CaravanLookAndFeel::drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                                               bool isSeparator, bool isActive, bool isHighlighted,
                                               bool isTicked, bool hasSubMenu, const juce::String &text,
                                               const juce::String &shortcutKeyText, const juce::Drawable *icon,
                                               const juce::Colour *textColour)
    {
        LookAndFeel_V4::drawPopupMenuItem(g, area, isSeparator, isActive, isHighlighted, isTicked, hasSubMenu, text, shortcutKeyText, icon, textColour);
    }

    void CaravanLookAndFeel::drawPopupMenuBackground(juce::Graphics &g, int width, int height)
    {
        LookAndFeel_V4::drawPopupMenuBackground(g, width, height);
    }

    void CaravanLookAndFeel::setFont(const juce::Font &newFont)
    {
        customFont = newFont;
    }
}