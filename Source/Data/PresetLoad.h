/*
  ==============================================================================

    PresetLoad.h
    Created: 4 Jan 2026 9:35:11am
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PresetLoad {
    public:
        PresetLoad(juce::AudioProcessorValueTreeState& apvts);

        void savePreset(const juce::String& name);
        void deletePreset(const juce::String& name);
        void loadPreset(const juce::String& name);

        juce::StringArray getAllPresets() const;
        juce::String getCurrentPreset() const { return currentPreset; } 
        
        juce::File getPresetFolder() const;

    private:
        juce::AudioProcessorValueTreeState& apvts;
        juce::String currentPreset;

        juce::File getDefaultPresetFolder() const;
        void pathCheck();
};