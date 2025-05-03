#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace Caravan
{
    class CaravanOpenGLContext;

    class CaravanLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CaravanLookAndFeel();
        ~CaravanLookAndFeel() override;

        void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                              float sliderPosProportional, float rotaryStartAngle,
                              float rotaryEndAngle, juce::Slider &slider) override;

        void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

        void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                          int buttonX, int buttonY, int buttonW, int buttonH,
                          juce::ComboBox &box) override;

        void positionComboBoxText(juce::ComboBox &box, juce::Label &label) override;

        void drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area,
                               bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                               const juce::String &text, const juce::String &shortcutKeyText,
                               const juce::Drawable *icon, const juce::Colour *textColour) override;

        void drawPopupMenuBackground(juce::Graphics &g, int width, int height) override;

        void drawLabel(juce::Graphics &g, juce::Label &label) override;

        void setFont(const juce::Font &newFont);

        juce::Colour getSandColor() const { return sandColor; }
        juce::Colour getDuneColor() const { return duneColor; }
        juce::Colour getSkyColor() const { return skyColor; }
        juce::Colour getSunColor() const { return sunColor; }

    private:
        void drawDesertKnob(juce::Graphics &g, int x, int y, int width, int height,
                            float sliderPosProportional, float rotaryStartAngle,
                            float rotaryEndAngle, bool isMainKnob);

        juce::Colour sandColor;   // Light sand
        juce::Colour duneColor;   // Darker sand for dunes
        juce::Colour skyColor;    // Desert sky blue
        juce::Colour sunColor;    // Orange sun color
        juce::Colour textColor;   // Text color
        juce::Colour shadowColor; // Shadow color

        juce::Image compassImage;        // Compass rose for main knob
        juce::Image knobImage;           // Small knob image
        juce::Image dialBackgroundImage; // Background for knobs
        juce::Font customFont;           // Custom font for text elements
    };

}