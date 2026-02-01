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
    // Initialize Stage 1 DSP modules for both channels
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

    // Initialize Stage 2 DSP modules for cascaded mode
    delayLine2Left.prepare(sampleRate, samplesPerBlock);
    delayLine2Right.prepare(sampleRate, samplesPerBlock);
    
    compander2Left.prepare(sampleRate);
    compander2Right.prepare(sampleRate);
    
    bbdModel2Left.prepare(sampleRate);
    bbdModel2Right.prepare(sampleRate);
    
    filter2Left.prepare(sampleRate);
    filter2Right.prepare(sampleRate);
    
    mixStage2Left.prepare(sampleRate);
    mixStage2Right.prepare(sampleRate);
}

void DM2DelayAudioProcessor::releaseResources()
{
    // Reset Stage 1 DSP modules
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

    // Reset Stage 2 DSP modules
    delayLine2Left.reset();
    delayLine2Right.reset();
    compander2Left.reset();
    compander2Right.reset();
    bbdModel2Left.reset();
    bbdModel2Right.reset();
    filter2Left.reset();
    filter2Right.reset();
    mixStage2Left.reset();
    mixStage2Right.reset();
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

    // Check bypass state - apply safety limiting even when bypassed
    if (isBypassed.load())
    {
        // Apply soft clipping to prevent digital clipping even in bypass
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] = std::tanh(channelData[sample]);
            }
        }
        return;
    }

    // Get parameter values for Stage 1
    float delayTime = apvts.getRawParameterValue(Parameters::delayTimeID)->load();
    float feedback = apvts.getRawParameterValue(Parameters::feedbackID)->load();
    float mix = apvts.getRawParameterValue(Parameters::mixID)->load();
    float tone = apvts.getRawParameterValue(Parameters::toneID)->load();

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
        
        // Select DSP modules for Stage 1
        DelayLine& delayLine = (channel == 0) ? delayLineLeft : delayLineRight;
        Compander& compander = (channel == 0) ? companderLeft : companderRight;
        BBDModel& bbdModel = (channel == 0) ? bbdModelLeft : bbdModelRight;
        Filter& filter = (channel == 0) ? filterLeft : filterRight;
        MixStage& mixStage = (channel == 0) ? mixStageLeft : mixStageRight;

        // Select DSP modules for Stage 2 (if needed)
        DelayLine& delayLine2 = (channel == 0) ? delayLine2Left : delayLine2Right;
        Compander& compander2 = (channel == 0) ? compander2Left : compander2Right;
        BBDModel& bbdModel2 = (channel == 0) ? bbdModel2Left : bbdModel2Right;
        Filter& filter2 = (channel == 0) ? filter2Left : filter2Right;
        MixStage& mixStage2 = (channel == 0) ? mixStage2Left : mixStage2Right;

        // Process sample-by-sample
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample];
            float drySample = inputSample; // Save for mixing
            
            // STAGE 1 DSP Chain
            // 1. Compressor (pre-BBD)
            float compressed = compander.compress(inputSample);
            
            // 2. BBD Delay
            float delayed = delayLine.processSample(compressed, delayTime, feedback);
            
            // 3. BBD artifacts
            float bbdProcessed = bbdModel.processSample(delayed, delayTime);
            
            // 4. Expander (post-BBD)
            float expanded = compander.expand(bbdProcessed);
            
            // 5. Filter stage
            float filtered = filter.processSample(expanded, tone);
            
            // 6. Mix stage 1 output
            float stage1Output = mixStage.processSample(drySample, filtered, mix);
            
            // STAGE 2: Cascaded processing (if in custom mode)
            float finalOutput = stage1Output;
            
            if (isCustomMode)
            {
                // Stage 2 processes the output of Stage 1 (true cascade)
                float stage2Input = stage1Output;
                
                // 1. Stage 2 Compressor
                float compressed2 = compander2.compress(stage2Input);
                
                // 2. Stage 2 BBD Delay  
                float delayed2 = delayLine2.processSample(compressed2, delayTime2, feedback2);
                
                // 3. Stage 2 BBD artifacts
                float bbdProcessed2 = bbdModel2.processSample(delayed2, delayTime2);
                
                // 4. Stage 2 Expander
                float expanded2 = compander2.expand(bbdProcessed2);
                
                // 5. Stage 2 Filter
                float filtered2 = filter2.processSample(expanded2, tone2);
                
                // 6. Mix stage 2: blend stage1 dry with stage 2 wet (not original input!)
                finalOutput = mixStage2.processSample(stage2Input, filtered2, mix2);
            }
            
            // Soft clip to prevent digital clipping
            finalOutput = std::tanh(finalOutput);
            
            channelData[sample] = finalOutput;
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
