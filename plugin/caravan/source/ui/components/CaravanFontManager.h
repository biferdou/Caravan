#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

namespace Caravan
{
    class CaravanFontManager
    {
    public:
        CaravanFontManager()
        {
            // Load custom font from binary resources
            int fontDataSize = 0;
            const char *fontData = nullptr;

            // Get the Cabin font from the binary resources
            fontData = BinaryData::getNamedResource("cabin_ttf", fontDataSize);

            if (fontData != nullptr && fontDataSize > 0)
            {
                customTypeface = juce::Typeface::createSystemTypefaceFor(fontData, fontDataSize);
                customFont = juce::Font(juce::FontOptions().withTypeface(customTypeface));
            }
            else
            {
                // Fallback to a system font with desert-like characteristics
                customFont = juce::Font(juce::FontOptions().withHeight(15.0f));
            }
        }

        ~CaravanFontManager() = default;

        // Get font with specified size
        juce::Font getFont(float fontSize) const
        {
            if (customTypeface != nullptr)
            {
                return juce::Font(juce::FontOptions().withTypeface(customTypeface).withHeight(fontSize));
            }

            // Fallback to system font
            return juce::Font(juce::FontOptions().withHeight(fontSize));
        }

        // Apply font to a component and all its children
        void applyFontToAllComponents(juce::Component *component, float fontSize)
        {
            if (component == nullptr)
                return;

            // If this is a Label, set its font
            juce::Label *label = dynamic_cast<juce::Label *>(component);
            if (label)
            {
                label->setFont(getFont(fontSize));
            }

            // Apply to all child components
            for (int i = 0; i < component->getNumChildComponents(); ++i)
            {
                applyFontToAllComponents(component->getChildComponent(i), fontSize);
            }
        }

    private:
        juce::Typeface::Ptr customTypeface;
        juce::Font customFont;
    };
}