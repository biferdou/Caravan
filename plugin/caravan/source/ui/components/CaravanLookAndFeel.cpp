#include "CaravanLookAndFeel.h"
#include <cmath>
#include "BinaryData.h"

namespace Caravan
{

    CaravanLookAndFeel::CaravanLookAndFeel()
    {
        sandColor = juce::Colours::sandybrown;
        duneColor = juce::Colours::saddlebrown;
        skyColor = juce::Colours::skyblue;
        sunColor = juce::Colours::orange;
        textColor = juce::Colours::black;
        shadowColor = juce::Colours::black.withAlpha(0.5f);

        customFont = juce::Font(juce::FontOptions().withHeight(20.0f));

        setColour(juce::Slider::thumbColourId, juce::Colours::orange);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkgrey);
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

        setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
        setColour(juce::ToggleButton::tickDisabledColourId, juce::Colours::orange.withAlpha(0.5f));
        setColour(juce::ToggleButton::textColourId, juce::Colours::black);

        setColour(juce::ComboBox::backgroundColourId, juce::Colours::darkgrey);
        setColour(juce::ComboBox::textColourId, juce::Colours::black);
        setColour(juce::ComboBox::arrowColourId, juce::Colours::orange);
        setColour(juce::ComboBox::buttonColourId, juce::Colours::darkgrey.darker());
        setColour(juce::ComboBox::outlineColourId, juce::Colours::darkgrey.darker());

        setColour(juce::Label::textColourId, juce::Colours::black);
        setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);

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
        if (slider.getName() == "Dust Drive" && compassImage.isValid())
        {
            auto bounds = juce::Rectangle<int>(x, y, width, height);
            auto center = bounds.getCentre().toFloat();
            float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
            float imageSize = juce::jmin(width, height) * 0.98f;
            float originalWidth = static_cast<float>(compassImage.getWidth());
            float originalHeight = static_cast<float>(compassImage.getHeight());
            float scale = imageSize / juce::jmax(originalWidth, originalHeight);

            juce::AffineTransform transform;
            transform = juce::AffineTransform::translation(-originalWidth / 2.0f, -originalHeight / 2.0f)
                            .rotated(angle)
                            .scaled(scale, scale)
                            .translated(center.x, center.y);

            g.drawImageTransformed(compassImage, transform);
            return;
        }

        float knobSize = slider.getName() == "Dust Drive" ? 1.0f : 0.75f;
        auto bounds = juce::Rectangle<float>(static_cast<float>(x),
                                             static_cast<float>(y),
                                             static_cast<float>(width),
                                             static_cast<float>(height));

        if (slider.getName() != "Dust Drive")
        {
            auto center = bounds.getCentre();
            float newSize = juce::jmin(bounds.getWidth(), bounds.getHeight()) * knobSize;
            bounds = juce::Rectangle<float>(center.x - newSize / 2.0f,
                                            center.y - newSize / 2.0f,
                                            newSize, newSize);
        }

        float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.4f;
        auto center = bounds.getCentre();

        g.setColour(juce::Colours::darkgrey);
        g.fillEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f);

        g.setColour(juce::Colours::grey);
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(slider.getName() == "Dust Drive" ? juce::Colours::orange : juce::Colours::orange.darker());
        juce::Path arc;
        arc.addArc(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f,
                   rotaryStartAngle, angle, true);
        g.strokePath(arc, juce::PathStrokeType(3.0f));

        juce::Path pointer;
        pointer.addRectangle(-1.5f, -radius, 3.0f, radius * 0.7f);
        pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(center.x, center.y));
        g.setColour(juce::Colours::black);
        g.fillPath(pointer);
    }

    void CaravanLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                                              bool shouldDrawButtonAsHighlighted,
                                              bool shouldDrawButtonAsDown)
    {
        juce::ignoreUnused(shouldDrawButtonAsDown);

        bool isOn = button.getToggleState();

        auto buttonArea = button.getLocalBounds();

        // Radio button design - circular
        const int radioSize = juce::jmin(buttonArea.getWidth(), buttonArea.getHeight() - 2);
        const float radius = radioSize / 2.0f;

        juce::Point<float> center(
            static_cast<float>(buttonArea.getCentreX()),
            static_cast<float>(buttonArea.getCentreY()));

        // Draw outer circle
        g.setColour(shouldDrawButtonAsHighlighted ? juce::Colours::black : juce::Colours::lightgrey);
        g.drawEllipse(center.x - radius, center.y - radius, radius * 2.0f, radius * 2.0f, 1.5f);

        // Draw background
        g.setColour(isOn ? juce::Colours::orange.darker() : juce::Colours::darkgrey);
        g.fillEllipse(center.x - radius + 1.5f, center.y - radius + 1.5f, (radius - 1.5f) * 2.0f, (radius - 1.5f) * 2.0f);

        // Draw inner dot if toggled on
        if (isOn)
        {
            g.setColour(juce::Colours::black);
            const float innerRadius = radius * 0.4f;
            g.fillEllipse(center.x - innerRadius, center.y - innerRadius, innerRadius * 2.0f, innerRadius * 2.0f);
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
        label.setFont(juce::Font(juce::FontOptions().withHeight(static_cast<float>(box.getHeight()) * 0.7f)));
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