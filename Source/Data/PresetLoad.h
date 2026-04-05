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
        /*
        * Constructor
		* @brief: Initializes the PresetLoad class initially check for preset directory path.
		* @params: juce::AudioProcessorValueTreeState& apvts: A reference to the AudioProcessorValueTreeState of the plugin, used to copy state and paste into a .xml preset file.
        */
        PresetLoad(juce::AudioProcessorValueTreeState& apvts);

        /* 
        * void savePreset(const juce::String& name)
		* @brief: Saves the current state of the plugin to a .xml file in the preset directory with the given name.
		* @params: const juce::String& name: The name of the preset to save.
        */
        void savePreset(const juce::String& name);
        /* 
        * void deletePreset(const juce::String& name)
		* @brief: Deletes the preset with the given name from the preset directory.
		* @param: const juce::String& name: The name of the preset to delete. This will be used as the filename for the .xml file.
        */
        void deletePreset(const juce::String& name);
        /*
        * void loadPreset(const juce::String& name)
		* @brief: Loads the preset with the given name from the preset directory and applies it to the plugin's state.
		* @param: const juce::String& name: The name of the preset to load. This will be used to find the .xml file in the preset directory, parse it, and apply the state to the plugin.
        */
        void loadPreset(const juce::String& name);
        /* 
        * juce::StringArray getAllPresets() const
		* @brief: Retrieves a list of all preset names available in the preset directory. This will look for .xml files in the preset directory and return their names without the .xml extension.
		* @return: juce::StringArray: An array of preset names available in the preset directory.
        */
        juce::StringArray getAllPresets() const;
        /* 
        * juce::String getCurrentPreset() const
		* @brief: Retrieves the name of the currently loaded preset. This will return the name of the preset that is currently applied to the plugin's state, or an empty string if no preset is currently loaded.
		* @return: juce::String: The name of the currently loaded preset, or an empty string if no preset is loaded.
        */
        juce::String getCurrentPreset() const { return currentPreset; } 
        /* 
        * juce::File getPresetFolder() const
		* @brief: Retrieves the file path of the preset directory. This will return a juce::File object representing the directory where presets are stored. If the directory does not exist, it will be created.
		* @return: Default preset directory is \Documents\SevenT\Presets on Windows OS. juce::File: A File object representing the preset directory.
        */
        juce::File getPresetFolder() const;

    private:
        juce::AudioProcessorValueTreeState& apvts;
        juce::String currentPreset;

        /* 
        * juce::File getDefaultPresetFolder()
		* @brief: Retrieves the default file path for the preset directory. \Documents\SevenT\Presets is the default directory on Windows OS. 		
        * @return: juce::File: A File object representing the default preset directory.
        */
        juce::File getDefaultPresetFolder() const;
        /* 
        * void pathCheck()
		* @brief: Checks if the preset directory exists, and if not, creates it.
        */
        void pathCheck();
};