#include "VocalEQ.h"

namespace Caravan
{

    VocalEQ::VocalEQ()
    {
        // Initialize filters with default coefficients
        auto defaultCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(44100.0f, 80.0f, 0.7f);

        *preEq.state = *defaultCoeffs;
        *deEsser.state = *defaultCoeffs;
        *airExciter.state = *defaultCoeffs;
    }

    void VocalEQ::prepare(const juce::dsp::ProcessSpec &spec)
    {
        sampleRate = static_cast<float>(spec.sampleRate);

        preEq.prepare(spec);
        deEsser.prepare(spec);
        airExciter.prepare(spec);

        reset();
    }

    void VocalEQ::reset()
    {
        preEq.reset();
        deEsser.reset();
        airExciter.reset();
    }

    void VocalEQ::updateSettings(bool tuneMode, float airValue, float deEsserValue)
    {
        if (tuneMode)
        {
            // When Tune Mode is on, apply vocal-optimized EQ curve
            // This is designed specifically for trap/hip-hop vocals that will have AutoTune applied

            // Low cut to remove rumble and mud (helps AutoTune tracking)
            // Modern vocal style with very little low-end
            *preEq.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(
                sampleRate, 120.0f, 0.7f);

            // Apply a shelf-style EQ boost in the 3-5kHz range for presence and clarity
            // This helps vocals cut through beats
            *airExciter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                sampleRate, 3800.0f, 0.8f, 1.0f + (0.5f + airValue * 0.5f));

            // Apply de-essing when in Tune Mode (using highshelf since notchFilter is unavailable)
            // This helps with AutoTune processing by removing harsh sibilance
            *deEsser.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                sampleRate, 7500.0f, 0.7f, 1.0f / (0.5f + deEsserValue * 0.7f));
        }
        else
        {
            // When Tune Mode is off, use more transparent settings

            // Gentler low cut
            *preEq.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(
                sampleRate, 80.0f, 0.7f);

            // More natural air/presence boost
            *airExciter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                sampleRate, 6500.0f, 0.7f, 1.0f + airValue * 1.5f);

            // Gentler de-essing (using highshelf with gain reduction)
            *deEsser.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                sampleRate, 7500.0f, 0.7f, 1.0f / (0.3f + deEsserValue * 0.5f));
        }
    }

    void VocalEQ::processBlock(juce::AudioBuffer<float> &buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        preEq.process(context);
        deEsser.process(context);
        airExciter.process(context);
    }

}