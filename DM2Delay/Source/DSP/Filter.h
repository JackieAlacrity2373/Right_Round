#pragma once

#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

/**
 * Filter - Lowpass filter for BBD clock noise removal and tone control
 * Implements biquad lowpass (3-5 kHz) plus variable tone control (3-8 kHz)
 * Based on design doc filter stage requirements
 */
class Filter
{
public:
    Filter();
    ~Filter() = default;

    /** Prepare for playback */
    void prepare(double sampleRate);

    /** Reset filter state */
    void reset();

    /**
     * Process a single sample through the filter chain
     * @param inputSample The input sample
     * @param tonePercent Tone control (0-100%, maps to 3-8 kHz cutoff)
     * @return Filtered output sample
     */
    float processSample(float inputSample, float tonePercent);

private:
    double currentSampleRate;
    
    // Two-stage filtering: BBD anti-aliasing + tone control
    juce::dsp::IIR::Filter<float> bhdLowpass;      // Fixed 5kHz BBD filter
    juce::dsp::IIR::Filter<float> toneControl;     // Variable tone filter
    
    float lastTonePercent;
    
    /** Update tone control filter coefficients */
    void updateToneFilter(float tonePercent);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Filter)
};
