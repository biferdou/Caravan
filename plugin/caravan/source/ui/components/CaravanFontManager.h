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
            int fontDataSize = 0;
            const char *fontData = nullptr;

            fontData = BinaryData::getNamedResource("changa_ttf", fontDataSize);

            if (fontData != nullptr && fontDataSize > 0)
            {
                customTypeface = juce::Typeface::createSystemTypefaceFor(fontData, fontDataSize);
                customFont = juce::Font(juce::FontOptions().withTypeface(customTypeface));
            }
            else
            {
                customFont = juce::Font(juce::FontOptions().withHeight(15.0f));
            }
        }

        ~CaravanFontManager() = default;

        juce::Font getFont(float fontSize) const
        {
            if (customTypeface != nullptr)
            {
                return juce::Font(juce::FontOptions().withTypeface(customTypeface).withHeight(fontSize));
            }

            return juce::Font(juce::FontOptions().withHeight(fontSize));
        }

        void applyFontToAllComponents(juce::Component *component, float fontSize)
        {
            if (component == nullptr)
                return;

            juce::Label *label = dynamic_cast<juce::Label *>(component);
            if (label)
            {
                label->setFont(getFont(fontSize));
            }

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