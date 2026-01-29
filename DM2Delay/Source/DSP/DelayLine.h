#pragma once

#include <juce_core/juce_core.h>
#include <vector>

/**
 * DelayLine - Fractional delay line with feedback
 * Implements circular buffer with cubic interpolation for smooth delay times
 * Based on design doc: 4096-stage delay with variable delay time (20-300ms)
 */
class DelayLine
{
public:
    DelayLine();
    ~DelayLine() = default;

    /** Prepare for playback with given sample rate and max buffer size */
    void prepare(double sampleRate, int maxBufferSize);

    /** Reset the delay line to silence */
    void reset();

    /** 
     * Process a single sample with feedback
     * @param inputSample The input sample to delay
     * @param delayTimeMs Delay time in milliseconds
     * @param feedback Feedback amount (0.0 to 0.95)
     * @return The delayed output sample
     */
    float processSample(float inputSample, float delayTimeMs, float feedback);

    /** Get the current delay time in samples */
    float getDelayInSamples(float delayTimeMs) const;

private:
    std::vector<float> buffer;
    int writeIndex;
    double currentSampleRate;
    int maxDelaySamples;

    /** Read from buffer with cubic interpolation */
    float readInterpolated(float delaySamples);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayLine)
};
