#include "CaravanLookAndFeel.h"

namespace Caravan
{

    CaravanLookAndFeel::CaravanLookAndFeel()
    {
        // Set desert-inspired color scheme
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

        // Draw the outer circle
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

            // Draw "ON" text
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
            // Draw "OFF" text when untoggled
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

}