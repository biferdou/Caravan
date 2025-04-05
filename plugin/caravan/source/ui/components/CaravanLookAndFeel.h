#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace Caravan
{

    class CaravanLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        CaravanLookAndFeel();

        void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                              float sliderPosProportional, float rotaryStartAngle,
                              float rotaryEndAngle, juce::Slider &slider) override;

        void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;
    };

}