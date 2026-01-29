#pragma once

#include <juce_core/juce_core.h>

/**
 * Compander - Compression/Expansion for BBD noise reduction
 * Implements 2:1 compression before delay, 2:1 expansion after
 * Based on design doc: NE570/SA571 dual compander emulation
 * Achieves ~10-15dB noise reduction
 */
class Compander
{
public:
    Compander();
    ~Compander() = default;

    /** Prepare for playback */
    void prepare(double sampleRate);

    /** Reset state */
    void reset();

    /**
     * Apply compression (pre-BBD)
     * @param inputSample Input signal
     * @return Compressed signal (boosted quiet, reduced loud)
     */
    float compress(float inputSample);

    /**
     * Apply expansion (post-BBD)
     * @param inputSample Compressed signal from BBD
     * @return Expanded signal (restored dynamics, reduced noise)
     */
    float expand(float inputSample);

private:
    double currentSampleRate;
    
    // Envelope followers for compression/expansion
    float compressEnvelope;
    float expandEnvelope;
    
    // Attack/release coefficients (from design doc: 1ms attack, 50ms release)
    float attackCoeff;
    float releaseCoeff;
    
    /** Update envelope follower */
    float updateEnvelope(float inputLevel, float currentEnvelope);
    
    /** Apply gain computation (2:1 ratio) */
    float computeGain(float envelope, bool isCompression);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compander)
};
