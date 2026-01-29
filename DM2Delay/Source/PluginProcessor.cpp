#include "PluginProcessor.h"
#include "PluginEditor.h"

DM2DelayAudioProcessor::DM2DelayAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", Parameters::createParameterLayout())
{
}

DM2DelayAudioProcessor::~DM2DelayAudioProcessor()
{
}

const juce::String DM2DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DM2DelayAudioProcessor::acceptsMidi() const
{
    return false;
}

bool DM2DelayAudioProcessor::producesMidi() const
{
    return false;
}

bool DM2DelayAudioProcessor::isMidiEffect() const
{
    return false;
}

double DM2DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DM2DelayAudioProcessor::getNumPrograms()
{
    return 1;
}

int DM2DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DM2DelayAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String DM2DelayAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void DM2DelayAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void DM2DelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Initialize all DSP modules for both channels
    delayLineLeft.prepare(sampleRate, samplesPerBlock);
    delayLineRight.prepare(sampleRate, samplesPerBlock);
    
    companderLeft.prepare(sampleRate);
    companderRight.prepare(sampleRate);
    
    bbdModelLeft.prepare(sampleRate);
    bbdModelRight.prepare(sampleRate);
    
    filterLeft.prepare(sampleRate);
    filterRight.prepare(sampleRate);
    
    mixStageLeft.prepare(sampleRate);
    mixStageRight.prepare(sampleRate);
}

void DM2DelayAudioProcessor::releaseResources()
{
    // Reset all DSP modules
    delayLineLeft.reset();
    delayLineRight.reset();
    companderLeft.reset();
    companderRight.reset();
    bbdModelLeft.reset();
    bbdModelRight.reset();
    filterLeft.reset();
    filterRight.reset();
    mixStageLeft.reset();
    mixStageRight.reset();
}

bool DM2DelayAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void DM2DelayAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Get parameter values
    float delayTime = apvts.getRawParameterValue(Parameters::delayTimeID)->load();
    float feedback = apvts.getRawParameterValue(Parameters::feedbackID)->load();
    float mix = apvts.getRawParameterValue(Parameters::mixID)->load();
    float tone = apvts.getRawParameterValue(Parameters::toneID)->load();
    // float modulation = apvts.getRawParameterValue(Parameters::modulationID)->load(); // Phase 3

    // Check if in custom mode (cascaded delay)
    bool isCustomMode = apvts.getRawParameterValue(Parameters::modeID)->load() > 0.5f;
    
    // Get stage 2 parameters if in custom mode
    float delayTime2 = 0.0f;
    float feedback2 = 0.0f;
    float mix2 = 0.0f;
    float tone2 = 0.0f;
    
    if (isCustomMode)
    {
        delayTime2 = apvts.getRawParameterValue(Parameters::delayTime2ID)->load();
        feedback2 = apvts.getRawParameterValue(Parameters::feedback2ID)->load();
        mix2 = apvts.getRawParameterValue(Parameters::mix2ID)->load();
        tone2 = apvts.getRawParameterValue(Parameters::tone2ID)->load();
    }

    // Process each channel
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        // Select DSP modules for this channel
        DelayLine& delayLine = (channel == 0) ? delayLineLeft : delayLineRight;
        Compander& compander = (channel == 0) ? companderLeft : companderRight;
        BBDModel& bbdModel = (channel == 0) ? bbdModelLeft : bbdModelRight;
        Filter& filter = (channel == 0) ? filterLeft : filterRight;
        MixStage& mixStage = (channel == 0) ? mixStageLeft : mixStageRight;

        // Process sample-by-sample (DSP chain as per design doc)
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            float drySample = inputSample; // Save for mixing
            
            // DSP Chain: Input → Compressor → Delay → BBD → Expander → Filter → Mix
            
            // 1. Input Stage (implicit - handled by DAW/plugin host)
            
            // 2. Compressor (pre-BBD)
            float compressed = compander.compress(inputSample);
            
            // 3. BBD Delay
            float delayed = delayLine.processSample(compressed, delayTime, feedback);
            
            // 4. BBD artifacts
            float bbdProcessed = bbdModel.processSample(delayed, delayTime);
            
            // 4.5. CUSTOM MODE: Cascade second delay stage (King of Tone style)
            if (isCustomMode)
            {
                // Run through delay chain again with stage 2 settings
                float compressed2 = compander.compress(bbdProcessed);
                float delayed2 = delayLine.processSample(compressed2, delayTime2, feedback2);
                bbdProcessed = bbdModel.processSample(delayed2, delayTime2);
                
                // Apply stage 2 filtering
                float expanded2 = compander.expand(bbdProcessed);
                float filtered2 = filter.processSample(expanded2, tone2);
                
                // Mix stage 2 with dry signal from stage 1 output
                bbdProcessed = mixStage.processSample(bbdProcessed, filtered2, mix2);
            }
            
            // 5. Expander (post-BBD)
            float expanded = compander.expand(bbdProcessed);
            
            // 6. Filter stage
            float filtered = filter.processSample(expanded, tone);
            
            // 7. Mix stage
            float output = mixStage.processSample(drySample, filtered, mix);
            
            channelData[sample] = output;
        }
    }
}

bool DM2DelayAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DM2DelayAudioProcessor::createEditor()
{
    return new DM2DelayAudioProcessorEditor(*this);
}

void DM2DelayAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DM2DelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DM2DelayAudioProcessor();
}
