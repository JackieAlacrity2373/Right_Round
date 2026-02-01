#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Parameters.h"
#include "DSP/DelayLine.h"
#include "DSP/Compander.h"
#include "DSP/BBDModel.h"
#include "DSP/Filter.h"
#include "DSP/MixStage.h"

/**
 * DM-2 Delay Audio Processor
 * Main plugin class implementing DSP chain as per design doc
 */
class DM2DelayAudioProcessor : public juce::AudioProcessor
{
public:
    DM2DelayAudioProcessor();
    ~DM2DelayAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Access to parameter tree
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    
    // Bypass control
    void setBypass(bool shouldBypass) { isBypassed.store(shouldBypass); }
    bool getBypass() const { return isBypassed.load(); }

private:
    juce::AudioProcessorValueTreeState apvts;

    // DSP modules - Stage 1: stereo processing (left/right channels)
    DelayLine delayLineLeft, delayLineRight;
    Compander companderLeft, companderRight;
    BBDModel bbdModelLeft, bbdModelRight;
    Filter filterLeft, filterRight;
    MixStage mixStageLeft, mixStageRight;

    // DSP modules - Stage 2: for cascaded mode (separate instances to avoid state corruption)
    DelayLine delayLine2Left, delayLine2Right;
    Compander compander2Left, compander2Right;
    BBDModel bbdModel2Left, bbdModel2Right;
    Filter filter2Left, filter2Right;
    MixStage mixStage2Left, mixStage2Right;

    // Bypass state
    std::atomic<bool> isBypassed{false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DM2DelayAudioProcessor)
};
