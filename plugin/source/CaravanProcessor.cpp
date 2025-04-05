#include "Caravan/CaravanProcessor.h"
#include "Caravan/CaravanEditor.h"

//==============================================================================
// DustDriveSaturator Implementation
void CaravanProcessor::DustDriveSaturator::prepare(const juce::dsp::ProcessSpec &spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    reset();
}

void CaravanProcessor::DustDriveSaturator::reset()
{
    // Reset any internal state if needed
}

float CaravanProcessor::DustDriveSaturator::processSample(float sample)
{
    // Coefficient values tuned based on NAV vocal characteristics
    const float preSaturationGain = 1.0f + driveAmount * 3.0f; // Range: 1.0 to 4.0
    const float positiveShaping = 1.2f;                        // More aggressive on positive peaks (consonants)
    const float negativeShaping = 0.8f;                        // Less aggressive on negative peaks (keeps warmth)
    const float highFreqEmphasis = 0.3f * driveAmount;         // Subtle high frequency enhancement

    // Apply pre-gain (pushes signal into saturation)
    float shaped = sample * preSaturationGain;

    // Get the sign before applying distortion
    float sign = shaped > 0.f ? 1.f : -1.f;
    float absValue = std::abs(shaped);

    // Calculate drive intensity (increases with input level for dynamic response)
    float drive = 1.0f + driveAmount * 6.0f; // Range: 1.0 to 7.0

    // Apply asymmetric saturation based on signal polarity
    float saturated;
    if (sign > 0)
    {
        // Tanh provides a smoother saturation curve than the previous exp function
        saturated = std::tanh(drive * absValue * positiveShaping);

        // Add subtle high-frequency emphasis on positive peaks (for presence)
        if (absValue > 0.1f)
        {
            saturated += highFreqEmphasis * saturated * (1.0f - std::exp(-absValue * 5.0f));
        }
    }
    else
    {
        // Softer curve on negative peaks to maintain warmth
        saturated = std::tanh(drive * absValue * negativeShaping);
    }

    saturated *= sign;

    // Apply drive-dependent limiting to prevent harsh clipping
    if (std::abs(saturated) > 0.95f)
    {
        saturated = sign * (0.95f + 0.05f * std::tanh((std::abs(saturated) - 0.95f) * 10.0f));
    }

    // Blend dry/wet based on drive amount for smooth control
    float wetMix = 0.4f + driveAmount * 0.6f; // Range: 0.4 to 1.0
    return sample * (1.0f - wetMix) + saturated * wetMix;
}

void CaravanProcessor::DustDriveSaturator::setDriveAmount(float newDriveAmount)
{
    driveAmount = newDriveAmount;
}

//==============================================================================
// CaravanProcessor Implementation
CaravanProcessor::CaravanProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Initialize parameter pointers
    dustDriveParam = apvts.getRawParameterValue("dustDrive");
    tuneModeParam = apvts.getRawParameterValue("tuneMode");
    widthParam = apvts.getRawParameterValue("width");
    airParam = apvts.getRawParameterValue("air");
    deEsserParam = apvts.getRawParameterValue("deEsser");

    // Initialize artist presets
    initializePresets();
}

CaravanProcessor::~CaravanProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout CaravanProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Main "Dust Drive" parameter
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "dustDrive", "Dust Drive", 0.0f, 1.0f, 0.5f));

    // Tune Mode switch
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "tuneMode", "Tune Mode", false));

    // Width control
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "width", "Width", 0.0f, 1.0f, 0.5f));

    // Air control
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "air", "Air", 0.0f, 1.0f, 0.5f));

    // De-esser control
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "deEsser", "De-Esser", 0.0f, 1.0f, 0.3f));

    return {params.begin(), params.end()};
}

void CaravanProcessor::initializePresets()
{
    artistPresets = {
        // NAV Preset - Characterized by moderate saturation, strong presence, and subtle width
        // Based on tracks like "Myself" and "Turks"
        {"NAV", 0.68f, true, 0.55f, 0.72f, 0.42f},

        // Playboi Carti Preset - More aggressive saturation, wide stereo image, less de-essing
        // Based on tracks like "Sky" and "Magnolia"
        {"Carti", 0.85f, true, 0.75f, 0.48f, 0.25f},

        // Travis Scott Preset - Medium saturation, controlled width, balanced air
        // Based on tracks like "SICKO MODE" and "goosebumps"
        {"Travis", 0.62f, true, 0.58f, 0.65f, 0.55f},

        // Future Preset - Heavy saturation, moderate width, emphasized air
        // Based on tracks like "Mask Off" and "Low Life"
        {"Future", 0.78f, true, 0.52f, 0.82f, 0.48f},

        // Don Toliver Preset - Medium saturation, widest stereo, spacious air
        // Based on tracks like "No Idea" and "After Party"
        {"Don Toliver", 0.65f, true, 0.82f, 0.74f, 0.40f},

        // The Weeknd Preset - Cleanest saturation, balanced width, strong air and presence
        // Based on tracks like "Blinding Lights" and "Starboy"
        {"The Weeknd", 0.45f, true, 0.60f, 0.78f, 0.62f},

        // Clean Preset - Minimal saturation, natural width, gentle presence enhancement
        // For a more transparent sound that still has some enhancement
        {"Clean", 0.25f, false, 0.40f, 0.35f, 0.70f},

        // Raw Preset - No saturation, minimal processing, just subtle enhancement
        // For when you want just a touch of improvement to the raw vocals
        {"Raw", 0.10f, false, 0.30f, 0.25f, 0.80f}};
}

void CaravanProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Prepare all DSP modules
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumInputChannels());

    inputGain.prepare(spec);
    preEq.prepare(spec);
    deEsser.prepare(spec);
    dustDrive.prepare(spec);
    stereoWidth.prepare(spec);
    airExciter.prepare(spec);
    outputGain.prepare(spec);

    // Initialize with default settings
    updateProcessorSettings();
}

void CaravanProcessor::updateProcessorSettings()
{
    // Get current parameter values
    float dustDriveValue = dustDriveParam->load();
    bool tuneMode = tuneModeParam->load() > 0.5f;
    float widthValue = widthParam->load();
    float airValue = airParam->load();
    float deEsserValue = deEsserParam->load();

    // Update Dust Drive
    dustDrive.setDriveAmount(dustDriveValue);

    // Update Tune Mode EQ settings
    if (tuneMode)
    {
        // When Tune Mode is on, apply vocal-optimized EQ curve
        // This is designed specifically for trap/hip-hop vocals that will have AutoTune applied

        // Low cut to remove rumble and mud (helps AutoTune tracking)
        // Modern NAV-style vocals have very little low-end
        *preEq.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(
            getSampleRate(), 120.0f, 0.7f);

        // Apply a shelf-style EQ boost in the 3-5kHz range for presence and clarity
        // This helps vocals cut through beats
        *airExciter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            getSampleRate(), 3800.0f, 0.8f, 1.0f + (0.5f + dustDriveValue * 0.5f));

        // Apply de-essing when in Tune Mode (using highshelf since notchFilter is unavailable)
        // This helps with AutoTune processing by removing harsh sibilance
        *deEsser.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
            getSampleRate(), 7500.0f, 0.7f, 1.0f / (0.5f + deEsserValue * 0.7f));
    }
    else
    {
        // When Tune Mode is off, use more transparent settings

        // Gentler low cut
        *preEq.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(
            getSampleRate(), 80.0f, 0.7f);

        // More natural air/presence boost
        *airExciter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
            getSampleRate(), 6500.0f, 0.7f, 1.0f + airValue * 1.5f);

        // Gentler de-essing (using highshelf with gain reduction since notchFilter is unavailable)
        *deEsser.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
            getSampleRate(), 7500.0f, 0.7f, 1.0f / (0.3f + deEsserValue * 0.5f));
    }

    // Update Width processing
    // NAV-style vocals often have subtle width enhancement
    stereoWidth.setPan(0.5f + (widthValue - 0.5f) * 0.5f);

    // Set input/output gain to prevent clipping
    // Adding a slight output gain compensation based on drive amount to maintain consistent levels
    inputGain.setGainLinear(0.8f);
    outputGain.setGainLinear(0.9f - dustDriveValue * 0.1f); // Compensate for saturation gain
}

void CaravanProcessor::releaseResources() {}

void CaravanProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &)
{
    juce::ScopedNoDenormals noDenormals;

    // Update processor settings for this block
    updateProcessorSettings();

    // Create audio block
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // Process the signal through our chain
    inputGain.process(context);
    preEq.process(context);
    deEsser.process(context);

    // Process Dust Drive (apply to all samples manually)
    processWithDustDrive(buffer);

    // Continue processing with width enhancement
    processWidth(buffer, widthParam->load());

    juce::dsp::AudioBlock<float> blockAfterWidth(buffer);
    juce::dsp::ProcessContextReplacing<float> contextAfterWidth(blockAfterWidth);

    airExciter.process(contextAfterWidth);
    outputGain.process(contextAfterWidth);
}

void CaravanProcessor::processWithDustDrive(juce::AudioBuffer<float> &buffer)
{
    // Get current drive value
    float driveAmount = dustDriveParam->load();

    // Higher drive values use multi-stage saturation for more extreme effect
    bool useMultistage = driveAmount > 0.7f;

    // Process each channel
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float *channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // For higher drive values, use multi-stage saturation
            if (useMultistage)
            {
                // First stage with moderate drive
                float firstStage = dustDrive.processSample(channelData[sample]);

                // Second stage with more drive for extreme settings
                channelData[sample] = dustDrive.processSample(firstStage);
            }
            else
            {
                // Single stage for cleaner settings
                channelData[sample] = dustDrive.processSample(channelData[sample]);
            }
        }
    }
}

void CaravanProcessor::processWidth(juce::AudioBuffer<float> &buffer, float widthAmount)
{
    if (buffer.getNumChannels() < 2)
        return;

    const float *leftIn = buffer.getReadPointer(0);
    const float *rightIn = buffer.getReadPointer(1);
    float *leftOut = buffer.getWritePointer(0);
    float *rightOut = buffer.getWritePointer(1);

    // Apply the width parameter using mid-side processing with frequency-dependent enhancement
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        // Extract mid and side signals
        float mid = (leftIn[i] + rightIn[i]) * 0.5f;
        float side = (leftIn[i] - rightIn[i]) * 0.5f;

        // Apply width enhancement
        // Map width from 0-1 to a more subtle range (0.8-1.5)
        // This gives more precise control in the useful range
        float sideGain = 0.8f + widthAmount * 0.7f;

        // Apply saturation to side signal for a more cohesive stereo image
        // This prevents the width effect from sounding artificial
        side = std::tanh(side * sideGain) / sideGain;

        // Convert back to left/right
        leftOut[i] = mid + side;
        rightOut[i] = mid - side;

        // Apply subtle compensation to maintain consistent volume across width settings
        float compensationGain = 1.0f - (widthAmount * 0.1f);
        leftOut[i] *= compensationGain;
        rightOut[i] *= compensationGain;
    }
}

void CaravanProcessor::applyPreset(int presetIndex)
{
    if (presetIndex >= 0 && presetIndex < artistPresets.size())
    {
        const auto &preset = artistPresets[presetIndex];

        // Apply preset values to parameters
        // Using setValueNotifyingHost ensures proper automation recording in the DAW
        apvts.getParameter("dustDrive")->setValueNotifyingHost(preset.dustDrive);
        apvts.getParameter("tuneMode")->setValueNotifyingHost(preset.tuneMode ? 1.0f : 0.0f);
        apvts.getParameter("width")->setValueNotifyingHost(preset.width);
        apvts.getParameter("air")->setValueNotifyingHost(preset.air);
        apvts.getParameter("deEsser")->setValueNotifyingHost(preset.deEsser);
    }
}

juce::AudioProcessorEditor *CaravanProcessor::createEditor()
{
    return new CaravanEditor(*this);
}

void CaravanProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void CaravanProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new CaravanProcessor();
}