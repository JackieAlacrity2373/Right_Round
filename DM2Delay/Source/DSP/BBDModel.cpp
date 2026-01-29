#include "BBDModel.h"

BBDModel::BBDModel()
    : currentSampleRate(44100.0)
    , noiseFloor(0.0f)
    , lastNoiseSample(0.0f)
{
    pinkFilterState[0] = 0.0f;
    pinkFilterState[1] = 0.0f;
    pinkFilterState[2] = 0.0f;
}

void BBDModel::prepare(double sampleRate)
{
    currentSampleRate = sampleRate;
    reset();
}

void BBDModel::reset()
{
    lastNoiseSample = 0.0f;
    pinkFilterState[0] = 0.0f;
    pinkFilterState[1] = 0.0f;
    pinkFilterState[2] = 0.0f;
}

float BBDModel::generateNoise(float delayTimeMs)
{
    // Noise floor increases with longer delay times (BBD characteristic)
    // Base noise at -60dB, increases slightly with delay time
    float baseNoiseDB = -60.0f;
    float delayFactor = delayTimeMs / 300.0f; // Normalize to max delay
    float noiseDB = baseNoiseDB + (delayFactor * 6.0f); // Up to -54dB at max delay
    
    float noiseAmplitude = std::pow(10.0f, noiseDB / 20.0f);
    
    // Generate white noise
    float white = random.nextFloat() * 2.0f - 1.0f;
    
    // Simple pink noise filter (Paul Kellet's approach)
    pinkFilterState[0] = 0.99765f * pinkFilterState[0] + white * 0.0990460f;
    pinkFilterState[1] = 0.96300f * pinkFilterState[1] + white * 0.2965164f;
    pinkFilterState[2] = 0.57000f * pinkFilterState[2] + white * 1.0526913f;
    
    float pink = pinkFilterState[0] + pinkFilterState[1] + pinkFilterState[2] + white * 0.1848f;
    pink *= 0.11f; // Normalize
    
    // Mix white and pink for BBD character (mostly pink)
    float noise = (pink * 0.8f + white * 0.2f) * noiseAmplitude;
    
    // Smooth noise slightly to avoid harsh digital artifacts
    noise = lastNoiseSample * 0.3f + noise * 0.7f;
    lastNoiseSample = noise;
    
    return noise;
}

float BBDModel::applySampleAndHold(float inputSample)
{
    // BBD sample-and-hold character is already handled by the delay line
    // and filtering stages, so we just add subtle quantization character
    
    // Very subtle bit-reduction effect (simulate BBD transfer non-linearity)
    const float steps = 4096.0f; // 12-bit equivalent
    float quantized = std::round(inputSample * steps) / steps;
    
    // Mix mostly original with subtle quantization
    return inputSample * 0.95f + quantized * 0.05f;
}

float BBDModel::processSample(float inputSample, float delayTimeMs)
{
    // Apply sample-and-hold character
    float processed = applySampleAndHold(inputSample);
    
    // Add BBD noise
    float noise = generateNoise(delayTimeMs);
    processed += noise;
    
    // Subtle soft clipping (BBD saturation)
    processed = std::tanh(processed * 0.9f) * 1.1f;
    
    return processed;
}
