#pragma once

#include <juce_core/juce_core.h>

/**
 * BBDModel - Bucket Brigade Device characteristics emulation
 * Adds BBD-specific artifacts: bandwidth limiting, noise, clock bleed
 * Based on design doc: MN3005 4096-stage BBD perceptual modeling
 */
class BBDModel
{
public:
    BBDModel();
    ~BBDModel() = default;

    /** Prepare for playback */
    void prepare(double sampleRate);

    /** Reset state */
    void reset();

    /**
     * Apply BBD character to a sample
     * @param inputSample The clean delayed sample
     * @param delayTimeMs Current delay time (affects noise floor)
     * @return Sample with BBD artifacts applied
     */
    float processSample(float inputSample, float delayTimeMs);

private:
    double currentSampleRate;
    juce::Random random;
    
    // BBD noise characteristics
    float noiseFloor;
    float lastNoiseSample;
    
    // Simple pink noise filter (1/f approximation)
    float pinkFilterState[3];
    
    /** Generate shaped BBD noise */
    float generateNoise(float delayTimeMs);
    
    /** Apply sample-and-hold character */
    float applySampleAndHold(float inputSample);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BBDModel)
};
