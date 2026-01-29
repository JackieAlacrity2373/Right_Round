#include "Compander.h"
#include <cmath>

Compander::Compander()
    : currentSampleRate(44100.0)
    , compressEnvelope(0.0f)
    , expandEnvelope(0.0f)
    , attackCoeff(0.0f)
    , releaseCoeff(0.0f)
{
}

void Compander::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    
    // Attack time: 1ms (fast response)
    float attackTimeMs = 1.0f;
    attackCoeff = 1.0f - std::exp(-1.0f / (attackTimeMs * 0.001f * static_cast<float>(sampleRate)));
    
    // Release time: 50ms (medium recovery)
    float releaseTimeMs = 50.0f;
    releaseCoeff = 1.0f - std::exp(-1.0f / (releaseTimeMs * 0.001f * static_cast<float>(sampleRate)));
    
    reset();
}

void Compander::reset()
{
    compressEnvelope = 0.0f;
    expandEnvelope = 0.0f;
}

float Compander::updateEnvelope(float inputLevel, float currentEnvelope)
{
    // Absolute value for envelope detection
    float absLevel = std::abs(inputLevel);
    
    // Attack if input > envelope, release if input < envelope
    float coeff = (absLevel > currentEnvelope) ? attackCoeff : releaseCoeff;
    
    // Smooth envelope follower
    return currentEnvelope + coeff * (absLevel - currentEnvelope);
}

float Compander::computeGain(float envelope, bool isCompression)
{
    // Threshold at -20dB (0.1 linear)
    const float threshold = 0.1f;
    
    // 2:1 compression ratio
    const float ratio = 2.0f;
    
    if (envelope < threshold)
        return 1.0f; // Below threshold, no change
    
    // Calculate gain reduction/expansion
    float envDB = 20.0f * std::log10(envelope + 1e-6f);
    float thresholdDB = 20.0f * std::log10(threshold);
    
    float gainChangeDB;
    if (isCompression)
    {
        // Compression: reduce dynamic range
        gainChangeDB = (envDB - thresholdDB) * (1.0f - 1.0f / ratio);
        gainChangeDB = -gainChangeDB; // Negative for reduction
    }
    else
    {
        // Expansion: restore dynamic range
        gainChangeDB = (envDB - thresholdDB) * (ratio - 1.0f);
    }
    
    // Convert dB to linear gain
    float gain = std::pow(10.0f, gainChangeDB / 20.0f);
    
    // Smooth limiting
    return juce::jlimit(0.1f, 3.0f, gain);
}

float Compander::compress(float inputSample)
{
    // Update envelope
    compressEnvelope = updateEnvelope(inputSample, compressEnvelope);
    
    // Calculate compression gain
    float gain = computeGain(compressEnvelope, true);
    
    // Apply gain
    return inputSample * gain;
}

float Compander::expand(float inputSample)
{
    // Update envelope
    expandEnvelope = updateEnvelope(inputSample, expandEnvelope);
    
    // Calculate expansion gain
    float gain = computeGain(expandEnvelope, false);
    
    // Apply gain
    return inputSample * gain;
}
