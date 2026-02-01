#include "DelayLine.h"

DelayLine::DelayLine()
    : writeIndex(0)
    , currentSampleRate(44100.0)
    , maxDelaySamples(0)
{
}

void DelayLine::prepare(double sampleRate, int maxBufferSize)
{
    juce::ignoreUnused(maxBufferSize);
    currentSampleRate = sampleRate;
    
    // Maximum delay time is 300ms (from design doc)
    maxDelaySamples = static_cast<int>(std::ceil(sampleRate * 0.3)) + 4; // +4 for interpolation safety
    
    buffer.resize(maxDelaySamples, 0.0f);
    reset();
}

void DelayLine::reset()
{
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
}

float DelayLine::getDelayInSamples(float delayTimeMs) const
{
    return (delayTimeMs / 1000.0f) * static_cast<float>(currentSampleRate);
}

float DelayLine::processSample(float inputSample, float delayTimeMs, float feedback)
{
    if (buffer.empty())
        return inputSample;
    
    // Clamp feedback to safe range (0-95% from design doc)
    feedback = juce::jlimit(0.0f, 0.95f, feedback / 100.0f);
    
    // Get delay in samples
    float delaySamples = getDelayInSamples(delayTimeMs);
    delaySamples = juce::jlimit(1.0f, static_cast<float>(maxDelaySamples - 4), delaySamples);
    
    // Read delayed sample with interpolation
    float delayedSample = readInterpolated(delaySamples);
    
    // Soft clip feedback to prevent runaway
    float feedbackSample = std::tanh(delayedSample * feedback);
    
    // Write input + feedback to buffer with soft clipping
    float bufferInput = std::tanh(inputSample + feedbackSample);
    
    jassert(writeIndex >= 0 && writeIndex < static_cast<int>(buffer.size()));
    buffer[writeIndex] = bufferInput;
    
    // Advance write index (circular buffer)
    writeIndex = (writeIndex + 1) % static_cast<int>(buffer.size());
    
    return delayedSample;
}

float DelayLine::readInterpolated(float delaySamples)
{
    if (buffer.empty())
        return 0.0f;
    
    int bufferSize = static_cast<int>(buffer.size());
    
    // Calculate read position
    float readPos = static_cast<float>(writeIndex) - delaySamples;
    
    // Wrap negative positions
    while (readPos < 0.0f)
        readPos += static_cast<float>(bufferSize);
    
    // Ensure readPos is within valid range
    while (readPos >= static_cast<float>(bufferSize))
        readPos -= static_cast<float>(bufferSize);
    
    // Get integer and fractional parts
    int index0 = static_cast<int>(std::floor(readPos)) % bufferSize;
    float frac = readPos - std::floor(readPos);
    
    // Get 4 samples for cubic interpolation with safe wrapping
    int index1 = (index0 + 1) % bufferSize;
    int index2 = (index0 + 2) % bufferSize;
    int indexMinus1 = (index0 - 1 + bufferSize) % bufferSize;
    
    // Bounds checking for safety
    jassert(indexMinus1 >= 0 && indexMinus1 < bufferSize);
    jassert(index0 >= 0 && index0 < bufferSize);
    jassert(index1 >= 0 && index1 < bufferSize);
    jassert(index2 >= 0 && index2 < bufferSize);
    
    float y0 = buffer[indexMinus1];
    float y1 = buffer[index0];
    float y2 = buffer[index1];
    float y3 = buffer[index2];
    
    // 4-point cubic interpolation (Hermite)
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);
    
    return ((c3 * frac + c2) * frac + c1) * frac + c0;
}
