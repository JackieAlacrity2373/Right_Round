#include "Filter.h"

Filter::Filter()
    : currentSampleRate(44100.0)
    , lastTonePercent(-1.0f)
{
}

void Filter::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    
    // Fixed BBD lowpass at 5 kHz (removes clock artifacts)
    auto bbdCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, 5000.0, 0.707); // Q = 0.707 (Butterworth)
    
    *bhdLowpass.coefficients = *bbdCoefficients;
    
    reset();
}

void Filter::reset()
{
    bhdLowpass.reset();
    toneControl.reset();
    lastTonePercent = -1.0f;
}

void Filter::updateToneFilter(float tonePercent)
{
    // Only update if tone changed significantly (avoid zipper noise)
    if (std::abs(tonePercent - lastTonePercent) < 0.1f)
        return;
    
    lastTonePercent = tonePercent;
    
    // Map 0-100% to 3-8 kHz cutoff frequency
    float cutoffHz = 3000.0f + (tonePercent / 100.0f) * 5000.0f;
    
    auto toneCoefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        currentSampleRate, cutoffHz, 0.707);
    
    *toneControl.coefficients = *toneCoefficients;
}

float Filter::processSample(float inputSample, float tonePercent)
{
    // Clamp tone to valid range
    tonePercent = juce::jlimit(0.0f, 100.0f, tonePercent);
    
    // Update tone filter if needed
    updateToneFilter(tonePercent);
    
    // Process through both filters
    float filtered = bhdLowpass.processSample(inputSample);
    filtered = toneControl.processSample(filtered);
    
    return filtered;
}
