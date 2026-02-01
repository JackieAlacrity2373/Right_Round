#include "PluginProcessor.h"
#include "PluginEditor.h"

DM2DelayAudioProcessorEditor::DM2DelayAudioProcessorEditor(DM2DelayAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Start with single pedal size
    setSize(400, 520);
    setResizable(false, false); // Fixed size based on mode

    // === MAIN CONTROLS (Top section knobs) ===
    
    // Delay Time knob
    delayTimeKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayTimeKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    addAndMakeVisible(delayTimeKnob);
    delayTimeLabel.setText("D.TIME", juce::dontSendNotification);
    delayTimeLabel.setJustificationType(juce::Justification::centred);
    delayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    delayTimeLabel.attachToComponent(&delayTimeKnob, false);
    addAndMakeVisible(delayTimeLabel);
    delayTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::delayTimeID, delayTimeKnob));

    // Feedback knob
    feedbackKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    addAndMakeVisible(feedbackKnob);
    feedbackLabel.setText("F.BACK", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    feedbackLabel.attachToComponent(&feedbackKnob, false);
    addAndMakeVisible(feedbackLabel);
    feedbackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::feedbackID, feedbackKnob));

    // Mix knob
    mixKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    addAndMakeVisible(mixKnob);
    mixLabel.setText("E.LEVEL", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    mixLabel.attachToComponent(&mixKnob, false);
    addAndMakeVisible(mixLabel);
    mixAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::mixID, mixKnob));

    // === INTERNAL TRIM CONTROLS (Smaller knobs) ===
    
    // Tone knob
    toneKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    toneKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(toneKnob);
    toneLabel.setText("TONE", juce::dontSendNotification);
    toneLabel.setJustificationType(juce::Justification::centred);
    toneLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    toneLabel.setFont(juce::Font(10.0f));
    toneLabel.attachToComponent(&toneKnob, false);
    addAndMakeVisible(toneLabel);
    toneAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::toneID, toneKnob));

    // Modulation knob
    modulationKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modulationKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(modulationKnob);
    modulationLabel.setText("MOD", juce::dontSendNotification);
    modulationLabel.setJustificationType(juce::Justification::centred);
    modulationLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    modulationLabel.setFont(juce::Font(10.0f));
    modulationLabel.attachToComponent(&modulationKnob, false);
    addAndMakeVisible(modulationLabel);
    modulationAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::modulationID, modulationKnob));

    // === STAGE 2 CONTROLS (Second pedal - same as stage 1) ===
    
    // Delay Time 2 knob
    delayTime2Knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayTime2Knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    addAndMakeVisible(delayTime2Knob);
    delayTime2Label.setText("D.TIME", juce::dontSendNotification);
    delayTime2Label.setJustificationType(juce::Justification::centred);
    delayTime2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    delayTime2Label.attachToComponent(&delayTime2Knob, false);
    addAndMakeVisible(delayTime2Label);
    delayTime2Attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::delayTime2ID, delayTime2Knob));

    // Feedback 2 knob
    feedback2Knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedback2Knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    addAndMakeVisible(feedback2Knob);
    feedback2Label.setText("F.BACK", juce::dontSendNotification);
    feedback2Label.setJustificationType(juce::Justification::centred);
    feedback2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    feedback2Label.attachToComponent(&feedback2Knob, false);
    addAndMakeVisible(feedback2Label);
    feedback2Attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::feedback2ID, feedback2Knob));

    // Mix 2 knob
    mix2Knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mix2Knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 18);
    addAndMakeVisible(mix2Knob);
    mix2Label.setText("E.LEVEL", juce::dontSendNotification);
    mix2Label.setJustificationType(juce::Justification::centred);
    mix2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    mix2Label.attachToComponent(&mix2Knob, false);
    addAndMakeVisible(mix2Label);
    mix2Attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::mix2ID, mix2Knob));

    // Tone 2 knob
    tone2Knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    tone2Knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(tone2Knob);
    tone2Label.setText("TONE", juce::dontSendNotification);
    tone2Label.setJustificationType(juce::Justification::centred);
    tone2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    tone2Label.setFont(juce::Font(10.0f));
    tone2Label.attachToComponent(&tone2Knob, false);
    addAndMakeVisible(tone2Label);
    tone2Attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::tone2ID, tone2Knob));

    // Modulation 2 knob
    modulation2Knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    modulation2Knob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(modulation2Knob);
    modulation2Label.setText("MOD", juce::dontSendNotification);
    modulation2Label.setJustificationType(juce::Justification::centred);
    modulation2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    modulation2Label.setFont(juce::Font(10.0f));
    modulation2Label.attachToComponent(&modulation2Knob, false);
    addAndMakeVisible(modulation2Label);
    modulation2Attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
        audioProcessor.getAPVTS(), Parameters::modulation2ID, modulation2Knob));

    // === BYPASS FOOTSWITCH ===
    bypassButton.setButtonText("");
    bypassButton.onClick = [this]()
    {
        isBypassed = !isBypassed;
        audioProcessor.setBypass(isBypassed);
        repaint();
    };
    addAndMakeVisible(bypassButton);

    // === MODE BUTTONS (STANDARD / CUSTOM) ===
    standardModeButton.setButtonText("");
    standardModeButton.onClick = [this]()
    {
        isCustomMode = false;
        audioProcessor.getAPVTS().getParameter(Parameters::modeID)->setValueNotifyingHost(0.0f);
        updateSizeForMode();
        repaint();
    };
    addAndMakeVisible(standardModeButton);

    customModeButton.setButtonText("");
    customModeButton.onClick = [this]()
    {
        isCustomMode = true;
        audioProcessor.getAPVTS().getParameter(Parameters::modeID)->setValueNotifyingHost(1.0f);
        updateSizeForMode();
        repaint();
    };
    addAndMakeVisible(customModeButton);
    
    // Initialize mode from parameter
    isCustomMode = audioProcessor.getAPVTS().getRawParameterValue(Parameters::modeID)->load() > 0.5f;
    updateSizeForMode();
    
    // Initialize bypass state from processor
    isBypassed = audioProcessor.getBypass();
}

DM2DelayAudioProcessorEditor::~DM2DelayAudioProcessorEditor()
{
}

void DM2DelayAudioProcessorEditor::updateSizeForMode()
{
    if (isCustomMode)
    {
        // Custom mode: Show two pedals side by side with cable connection
        setSize(800, 520); // 2x width for dual pedals
    }
    else
    {
        // Standard mode: Single pedal
        setSize(400, 520);
    }
    resized();
}

void DM2DelayAudioProcessorEditor::drawInputJack(juce::Graphics& g, int x, int y)
{
    // Input jack (1/4" phone jack)
    g.setColour(juce::Colours::black);
    g.fillEllipse(x - 15, y - 15, 30, 30);
    g.setColour(juce::Colour(0xff606060));
    g.fillEllipse(x - 12, y - 12, 24, 24);
    g.setColour(juce::Colours::black);
    g.fillEllipse(x - 6, y - 6, 12, 12);
    
    // Label
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(9.0f));
    g.drawText("INPUT", x - 20, y + 18, 40, 12, juce::Justification::centred);
}

void DM2DelayAudioProcessorEditor::drawOutputJack(juce::Graphics& g, int x, int y)
{
    // Output jack (1/4" phone jack)
    g.setColour(juce::Colours::black);
    g.fillEllipse(x - 15, y - 15, 30, 30);
    g.setColour(juce::Colour(0xff606060));
    g.fillEllipse(x - 12, y - 12, 24, 24);
    g.setColour(juce::Colours::black);
    g.fillEllipse(x - 6, y - 6, 12, 12);
    
    // Label
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(9.0f));
    g.drawText("OUTPUT", x - 20, y + 18, 40, 12, juce::Justification::centred);
}

void DM2DelayAudioProcessorEditor::drawLED(juce::Graphics& g, int x, int y, bool isOn)
{
    // LED bezel (chrome ring)
    g.setColour(juce::Colour(0xffa0a0a0));
    g.fillEllipse(x - 8, y - 8, 16, 16);
    
    // LED itself
    if (isOn)
    {
        // Bright red when on
        g.setColour(juce::Colour(0xffff0000));
        g.fillEllipse(x - 6, y - 6, 12, 12);
        // Glow effect
        g.setColour(juce::Colour(0x80ff0000));
        g.fillEllipse(x - 8, y - 8, 16, 16);
    }
    else
    {
        // Dark red when off
        g.setColour(juce::Colour(0xff3a0000));
        g.fillEllipse(x - 6, y - 6, 12, 12);
    }
}

void DM2DelayAudioProcessorEditor::drawFootswitch(juce::Graphics& g, juce::Rectangle<int> area, bool isPressed)
{
    auto centerX = area.getCentreX();
    auto centerY = area.getCentreY();
    
    // Shadow/base
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    g.fillEllipse(centerX - 42, centerY - 38 + (isPressed ? 2 : 0), 84, 84);
    
    // Footswitch body (metallic)
    juce::ColourGradient gradient(
        juce::Colour(0xffc0c0c0), centerX - 30, centerY - 30,
        juce::Colour(0xff707070), centerX + 30, centerY + 30,
        true);
    g.setGradientFill(gradient);
    g.fillEllipse(centerX - 40, centerY - 40 + (isPressed ? 2 : 0), 80, 80);
    
    // Top highlight
    g.setColour(juce::Colour(0xffe0e0e0));
    g.fillEllipse(centerX - 25, centerY - 35 + (isPressed ? 2 : 0), 50, 20);
    
    // Center circle
    g.setColour(juce::Colour(0xff404040));
    g.fillEllipse(centerX - 15, centerY - 15 + (isPressed ? 2 : 0), 30, 30);
}

void DM2DelayAudioProcessorEditor::drawSignalCable(juce::Graphics& g, int x1, int y1, int x2, int y2)
{
    // Draw instrument cable connecting two pedals horizontally
    juce::Path cablePath;
    cablePath.startNewSubPath(static_cast<float>(x1), static_cast<float>(y1));
    
    // Add slight curve for realistic cable sag (downward curve in middle)
    float midX = (x1 + x2) / 2.0f;
    float sagAmount = 40.0f; // How much the cable droops
    cablePath.cubicTo(
        static_cast<float>(x1 + 50), static_cast<float>(y1 + sagAmount),
        static_cast<float>(x2 - 50), static_cast<float>(y2 + sagAmount),
        static_cast<float>(x2), static_cast<float>(y2)
    );
    
    // Cable outer sheath (black)
    g.setColour(juce::Colours::black);
    g.strokePath(cablePath, juce::PathStrokeType(8.0f));
    
    // Cable inner (dark gray)
    g.setColour(juce::Colour(0xff404040));
    g.strokePath(cablePath, juce::PathStrokeType(6.0f));
}

void DM2DelayAudioProcessorEditor::drawPedalInstance(juce::Graphics& g, int xOffset, bool isSecondInstance)
{
    auto pedalArea = juce::Rectangle<int>(xOffset, 0, 400, 520);
    
    // Legally distinct red (burgundy/maroon)
    juce::Colour pedalRed(0xffa8273e);

    // Background - pedal enclosure
    g.setColour(pedalRed);
    g.fillRect(pedalArea);
    
    // Pedal border/edge shading
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.drawRect(pedalArea, 8);
    
    // Top highlight
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.fillRect(xOffset + 10, 10, 380, 40);
    
    // === LED INDICATOR (top center) ===
    drawLED(g, xOffset + 200, 35, !isBypassed);
    
    // === TITLE TEXT ===
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(22.0f, juce::Font::bold));
    g.drawText("ANALOG", xOffset + 50, 60, 300, 30, juce::Justification::centred);
    
    g.setFont(juce::Font(28.0f, juce::Font::bold));
    g.drawText("DELAY", xOffset + 50, 85, 300, 35, juce::Justification::centred);
    
    // Subtitle
    g.setFont(juce::Font(11.0f));
    if (isSecondInstance)
        g.drawText("VINTAGE CIRCUIT - STAGE 2", xOffset + 50, 120, 300, 15, juce::Justification::centred);
    else
        g.drawText("VINTAGE CIRCUIT", xOffset + 50, 120, 300, 15, juce::Justification::centred);
    
    // === INPUT/OUTPUT JACKS ===
    drawInputJack(g, xOffset + 30, 250);
    drawOutputJack(g, xOffset + 370, 250);
    
    // === INTERNAL SECTION INDICATOR ===
    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(juce::Font(9.0f));
    g.drawText("INTERNAL TRIM", xOffset + 20, 295, 100, 12, juce::Justification::centred);
    g.drawLine(static_cast<float>(xOffset + 30), 306.0f, static_cast<float>(xOffset + 90), 306.0f, 0.5f);
    
    // === MODE INDICATORS (only on first pedal) ===
    if (!isSecondInstance)
    {
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(10.0f, juce::Font::bold));
        
        // Standard mode indicator
        g.drawText("STANDARD", xOffset + 130, 245, 70, 12, juce::Justification::centred);
        
        if (!isCustomMode)
        {
            g.setColour(juce::Colour(0xffff0000));
            g.fillEllipse(static_cast<float>(xOffset + 145), 260.0f, 8.0f, 8.0f);
        }
        else
        {
            g.setColour(juce::Colours::white);
            g.drawEllipse(static_cast<float>(xOffset + 145), 260.0f, 8.0f, 8.0f, 1.5f);
        }
        
        // Custom mode indicator  
        g.setColour(juce::Colours::white);
        g.drawText("CUSTOM", xOffset + 200, 245, 70, 12, juce::Justification::centred);
        
        if (isCustomMode)
        {
            g.setColour(juce::Colour(0xffff0000));
            g.fillEllipse(static_cast<float>(xOffset + 220), 260.0f, 8.0f, 8.0f);
        }
        else
        {
            g.setColour(juce::Colours::white);
            g.drawEllipse(static_cast<float>(xOffset + 220), 260.0f, 8.0f, 8.0f, 1.5f);
        }
    }
    
    // Footswitch at bottom
    drawFootswitch(g, juce::Rectangle<int>(xOffset, 380, 400, 100), isBypassed);
    
    // Bottom text
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(8.0f));
    if (isSecondInstance)
        g.drawText("Cascaded Stage", xOffset + 150, 505, 100, 15, juce::Justification::centred);
    else
        g.drawText("Made with JUCE", xOffset + 150, 505, 100, 15, juce::Justification::centred);
}

void DM2DelayAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xff1a1a1a));
    
    if (isCustomMode)
    {
        // Draw two pedals side-by-side with cable connection
        
        // First pedal (left)
        drawPedalInstance(g, 0, false);
        
        // Second pedal (right)
        drawPedalInstance(g, 400, true);
        
        // Signal cable from first pedal output to second pedal input
        int cable1X = 370;  // Output of first pedal (left)
        int cable1Y = 250;
        int cable2X = 430;  // Input of second pedal (right)  
        int cable2Y = 250;
        
        drawSignalCable(g, cable1X, cable1Y, cable2X, cable2Y);
    }
    else
    {
        // Draw single pedal
        drawPedalInstance(g, 0, false);
    }
}

void DM2DelayAudioProcessorEditor::resized()
{
    // Main knobs (top row)
    int knobSize = 85;
    int knobY = 150;
    int knobSpacing = 120;
    
    // In custom mode, position knobs on first pedal (left side)
    // In standard mode, center them in the single pedal
    int pedalWidth = 400;
    int startX = (pedalWidth - (knobSpacing * 2 + knobSize)) / 2;

    delayTimeKnob.setBounds(startX, knobY, knobSize, knobSize);
    feedbackKnob.setBounds(startX + knobSpacing, knobY, knobSize, knobSize);
    mixKnob.setBounds(startX + knobSpacing * 2, knobY, knobSize, knobSize);

    // Internal trim knobs (smaller, below main controls)
    int trimSize = 45;
    int trimY = 310;
    
    toneKnob.setBounds(35, trimY, trimSize, trimSize);
    modulationKnob.setBounds(35, trimY + 55, trimSize, trimSize);

    // Stage 2 controls (second pedal - same positions but offset by 400px horizontally)
    int stage2Offset = 400;
    delayTime2Knob.setBounds(stage2Offset + startX, knobY, knobSize, knobSize);
    feedback2Knob.setBounds(stage2Offset + startX + knobSpacing, knobY, knobSize, knobSize);
    mix2Knob.setBounds(stage2Offset + startX + knobSpacing * 2, knobY, knobSize, knobSize);
    
    tone2Knob.setBounds(stage2Offset + 35, trimY, trimSize, trimSize);
    modulation2Knob.setBounds(stage2Offset + 35, trimY + 55, trimSize, trimSize);
    
    // Hide/show stage 2 controls based on mode
    delayTime2Knob.setVisible(isCustomMode);
    feedback2Knob.setVisible(isCustomMode);
    mix2Knob.setVisible(isCustomMode);
    tone2Knob.setVisible(isCustomMode);
    modulation2Knob.setVisible(isCustomMode);

    // Footswitch button (invisible clickable area) - on first pedal
    bypassButton.setBounds(pedalWidth / 2 - 50, 400, 100, 100);
    bypassButton.setAlpha(0.0f); // Invisible, just for clicking

    // Mode button clickable areas - on first pedal
    standardModeButton.setBounds(130, 245, 70, 30);
    standardModeButton.setAlpha(0.0f);
    
    customModeButton.setBounds(200, 245, 70, 30);
    customModeButton.setAlpha(0.0f);
}
