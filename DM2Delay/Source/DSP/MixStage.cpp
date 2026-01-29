#include "MixStage.h"
#include <cmath>

MixStage::MixStage()
    : currentSampleRate(44100.0)
    , lastMixPercent(-1.0f)
    , smoothedMix(0.0f)
    , smoothingCoeff(0.0f)
{
}

void MixStage::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    
    // Smoothing coefficient for parameter changes (5ms smoothing time)
    float smoothingTimeMs = 5.0f;
    smoothingCoeff = 1.0f - std::exp(-1.0f / (smoothingTimeMs * 0.001f * static_cast<float>(sampleRate)));
    
    reset();
}

void MixStage::reset()
{
    smoothedMix = 0.0f;
    lastMixPercent = -1.0f;
}

float MixStage::processSample(float drySample, float wetSample, float mixPercent)
{
    // Clamp mix to valid range
    mixPercent = juce::jlimit(0.0f, 100.0f, mixPercent);
    
    // Convert percentage to 0-1 range
    float mixNormalized = mixPercent / 100.0f;
    
    // Smooth parameter changes
    smoothedMix += smoothingCoeff * (mixNormalized - smoothedMix);
    
    // Equal-power crossfade (preserves perceived loudness)
    // Using sine/cosine law for smooth crossfade
    float wetGain = std::sin(smoothedMix * juce::MathConstants<float>::halfPi);
    float dryGain = std::cos(smoothedMix * juce::MathConstants<float>::halfPi);
    
    // Mix signals
    float output = drySample * dryGain + wetSample * wetGain;
    
    return output;
}
