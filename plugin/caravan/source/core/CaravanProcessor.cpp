#include "core/CaravanProcessor.h"
#include "core/AudioParameters.h"
#include "ui/editor/CaravanEditor.h"

namespace Caravan
{

    CaravanProcessor::CaravanProcessor()
        : AudioProcessor(BusesProperties()
                             .withInput("Input", juce::AudioChannelSet::stereo(), true)
                             .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
          apvts(*this, nullptr, "Parameters", AudioParameters::createParameterLayout())
    {
        // Initialize parameter pointers
        dustDriveParam = apvts.getRawParameterValue("dustDrive");
        tuneModeParam = apvts.getRawParameterValue("tuneMode");
        widthParam = apvts.getRawParameterValue("width");
        airParam = apvts.getRawParameterValue("air");
        deEsserParam = apvts.getRawParameterValue("deEsser");

        // Initialize preset manager
        presetManager = std::make_unique<PresetManager>(apvts);
    }

    CaravanProcessor::~CaravanProcessor() {}

    void CaravanProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        // Prepare all DSP modules
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
        spec.numChannels = static_cast<juce::uint32>(getTotalNumInputChannels());

        inputGain.prepare(spec);
        vocalEQ.prepare(spec);
        dustDrive.prepare(spec);
        stereoWidth.prepare(spec);
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

        // Update Vocal EQ settings
        vocalEQ.updateSettings(tuneMode, airValue, deEsserValue);

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

        // Create audio block for initial and final processing
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);

        // Process input gain
        inputGain.process(context);

        // Process vocal EQ (includes preEQ, deEsser, airExciter)
        vocalEQ.processBlock(buffer);

        // Process Dust Drive saturation
        dustDrive.processBlock(buffer);

        // Process width enhancement
        stereoWidth.processBlock(buffer, widthParam->load());

        // Process output gain
        outputGain.process(context);
    }

    void CaravanProcessor::applyPreset(int presetIndex)
    {
        presetManager->applyPreset(presetIndex);
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

} // namespace Caravan

// Function to create the plugin instance
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new Caravan::CaravanProcessor();
}