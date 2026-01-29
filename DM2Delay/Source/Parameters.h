#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Parameter IDs and ranges for DM-2 Delay plugin
 * Following design spec: Delay Time, Feedback, Mix, Tone, Modulation
 */
namespace Parameters
{
    // Parameter IDs - Stage 1 (first pedal)
    const juce::String delayTimeID = "delayTime";
    const juce::String feedbackID = "feedback";
    const juce::String mixID = "mix";
    const juce::String toneID = "tone";
    const juce::String modulationID = "modulation";
    
    // Mode parameter
    const juce::String modeID = "mode"; // 0 = Standard, 1 = Custom
    
    // Parameter IDs - Stage 2 (second pedal in cascaded mode)
    const juce::String delayTime2ID = "delayTime2";
    const juce::String feedback2ID = "feedback2";
    const juce::String mix2ID = "mix2";
    const juce::String tone2ID = "tone2";
    const juce::String modulation2ID = "modulation2";

    // Parameter ranges (from design doc)
    const float delayTimeMin = 20.0f;    // ms
    const float delayTimeMax = 300.0f;   // ms
    const float delayTimeDefault = 100.0f;

    const float feedbackMin = 0.0f;      // %
    const float feedbackMax = 95.0f;     // %
    const float feedbackDefault = 30.0f;

    const float mixMin = 0.0f;           // %
    const float mixMax = 100.0f;         // %
    const float mixDefault = 50.0f;

    const float toneMin = 0.0f;          // %
    const float toneMax = 100.0f;        // %
    const float toneDefault = 70.0f;

    const float modulationMin = 0.0f;    // Hz
    const float modulationMax = 10.0f;   // Hz
    const float modulationDefault = 0.0f;

    // Helper to create parameter layout
    inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            delayTimeID, "Delay Time",
            juce::NormalisableRange<float>(delayTimeMin, delayTimeMax, 1.0f),
            delayTimeDefault,
            "ms"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            feedbackID, "Feedback",
            juce::NormalisableRange<float>(feedbackMin, feedbackMax, 0.1f),
            feedbackDefault,
            "%"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            mixID, "Mix",
            juce::NormalisableRange<float>(mixMin, mixMax, 0.1f),
            mixDefault,
            "%"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            toneID, "Tone",
            juce::NormalisableRange<float>(toneMin, toneMax, 0.1f),
            toneDefault,
            "%"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            modulationID, "Modulation",
            juce::NormalisableRange<float>(modulationMin, modulationMax, 0.1f),
            modulationDefault,
            "Hz"));

        // Mode parameter
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            modeID, "Mode",
            juce::NormalisableRange<float>(0.0f, 1.0f, 1.0f),
            0.0f, // Default to standard mode
            ""));

        // Stage 2 parameters (for cascaded mode)
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            delayTime2ID, "Delay Time 2",
            juce::NormalisableRange<float>(delayTimeMin, delayTimeMax, 1.0f),
            delayTimeDefault,
            "ms"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            feedback2ID, "Feedback 2",
            juce::NormalisableRange<float>(feedbackMin, feedbackMax, 0.1f),
            feedbackDefault,
            "%"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            mix2ID, "Mix 2",
            juce::NormalisableRange<float>(mixMin, mixMax, 0.1f),
            mixDefault,
            "%"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            tone2ID, "Tone 2",
            juce::NormalisableRange<float>(toneMin, toneMax, 0.1f),
            toneDefault,
            "%"));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            modulation2ID, "Modulation 2",
            juce::NormalisableRange<float>(modulationMin, modulationMax, 0.1f),
            modulationDefault,
            "Hz"));

        return layout;
    }
}
