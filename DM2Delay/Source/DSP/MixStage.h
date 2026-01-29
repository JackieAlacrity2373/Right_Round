#pragma once

#include <juce_core/juce_core.h>

/**
 * MixStage - Dry/Wet blending with level compensation
 * Implements final mixing stage with equal-power crossfade
 * Based on design doc: 0-100% mix with perceived loudness preservation
 */
class MixStage
{
public:
    MixStage();
    ~MixStage() = default;

    /** Prepare for playback */
    void prepare(double sampleRate);

    /** Reset state */
    void reset();

    /**
     * Mix dry and wet signals
     * @param drySample Original input signal
     * @param wetSample Processed (delayed) signal
     * @param mixPercent Mix amount (0-100%: 0=all dry, 100=all wet)
     * @return Mixed output sample
     */
    float processSample(float drySample, float wetSample, float mixPercent);

private:
    double currentSampleRate;
    
    // Smooth parameter changes to avoid zipper noise
    float lastMixPercent;
    float smoothedMix;
    float smoothingCoeff;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MixStage)
};
