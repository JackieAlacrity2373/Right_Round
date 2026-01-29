#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"

/**
 * DM-2 Delay Editor (UI)
 * Vintage delay pedal interface inspired by classic analog designs
 * External: Delay Time, Feedback, Mix (main user controls)
 * Internal: Tone, Modulation (trim pots for fine-tuning)
 */
class DM2DelayAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DM2DelayAudioProcessorEditor(DM2DelayAudioProcessor&);
    ~DM2DelayAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // Get current mode state
    bool isInCustomMode() const { return isCustomMode; }
    
    // Update window size based on mode
    void updateSizeForMode();

private:
    DM2DelayAudioProcessor& audioProcessor;

    // Bypass and mode state
    bool isBypassed = false;
    bool isCustomMode = false; // false = Standard, true = Custom (cascaded)

    // Stage 1 controls (first pedal)
    juce::Slider delayTimeKnob;
    juce::Label delayTimeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;

    juce::Slider feedbackKnob;
    juce::Label feedbackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;

    juce::Slider mixKnob;
    juce::Label mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    juce::Slider toneKnob;
    juce::Label toneLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;

    juce::Slider modulationKnob;
    juce::Label modulationLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modulationAttachment;

    // Stage 2 controls (second pedal in cascaded mode)
    juce::Slider delayTime2Knob;
    juce::Label delayTime2Label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTime2Attachment;

    juce::Slider feedback2Knob;
    juce::Label feedback2Label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedback2Attachment;

    juce::Slider mix2Knob;
    juce::Label mix2Label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mix2Attachment;

    juce::Slider tone2Knob;
    juce::Label tone2Label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> tone2Attachment;

    juce::Slider modulation2Knob;
    juce::Label modulation2Label;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> modulation2Attachment;

    // Footswitch button
    juce::TextButton bypassButton;

    // Mode buttons
    juce::TextButton standardModeButton;
    juce::TextButton customModeButton;

    // Helper methods
    void drawInputJack(juce::Graphics& g, int x, int y);
    void drawOutputJack(juce::Graphics& g, int x, int y);
    void drawLED(juce::Graphics& g, int x, int y, bool isOn);
    void drawFootswitch(juce::Graphics& g, juce::Rectangle<int> area, bool isPressed);
    void drawPedalInstance(juce::Graphics& g, int xOffset, bool isSecondInstance);
    void drawSignalCable(juce::Graphics& g, int x1, int y1, int x2, int y2);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DM2DelayAudioProcessorEditor)
};
