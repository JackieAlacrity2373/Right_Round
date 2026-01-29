cmake_minimum_required(VERSION 3.15)

project(DM2Delay VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE to the project
add_subdirectory(../JUCE JUCE)

# Enable all plugin formats you want to build
juce_add_plugin(DM2Delay
    COMPANY_NAME "RightRound"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Rtrn
    PLUGIN_CODE Dm2d
    FORMATS VST3 Standalone
    PRODUCT_NAME "DM-2 Delay")

# Add source files
target_sources(DM2Delay PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/Parameters.h
    Source/DSP/DelayLine.cpp
    Source/DSP/DelayLine.h
    Source/DSP/Compander.cpp
    Source/DSP/Compander.h
    Source/DSP/BBDModel.cpp
    Source/DSP/BBDModel.h
    Source/DSP/Filter.cpp
    Source/DSP/Filter.h
    Source/DSP/MixStage.cpp
    Source/DSP/MixStage.h)

# Link JUCE libraries
target_compile_definitions(DM2Delay PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(DM2Delay PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
