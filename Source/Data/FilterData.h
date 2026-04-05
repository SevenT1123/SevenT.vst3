/*
  ==============================================================================

    FilterData.h
    Created: 23 Jan 2025 7:31:57pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FilterData {
    public:
        /* 
        * void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
		* @brief: Prepares the filter for playback and pass the parameters to the JUCE DSP filter.
		* @params: double sampleRate: the sample rate of the audio signal. int samplesPerBlock: the number of samples that will be processed in each block. int numChannels: the number of audio channels.
        */
        void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
        /* 
        * void process(juce::AudioBuffer<float>& buffer);
		* @brief: Process the audio buffer throught the filter and apply the filter to the audio signal using JUCE DSP StateVariableTPTFilter.
		* @param: juce::AudioBuffer<float>& buffer: the audio buffer that will be processed and filtered.
        */
        void process(juce::AudioBuffer<float>& buffer);
        /* 
        * void updateParameters(const int filterType, const float frequency, const float resonance)
		* @brief: Update the filter parameters based on the user input and set the filter type, cutoff frequency, and resonance using the JUCE DSP StateVariableTPTFilter.
		* @params: const int filterType: the type of the filter (bypass, lowpass, bandpass, highpass). const float frequency: the cutoff frequency of the filter. const float resonance: the resonance of the filter.
        */
        void updateParameters(const int filterType, const float frequency, const float resonance);
        /*
        * void reset()
		* @brief: reset the filter state and clear any internal buffers or state variables to ensure that the filter starts processing with a clean slate. This is typically called when the user changes the filter type to bypass or when the plugin is reset to ensure that any previous filter settings do not affect the new filter configuration.
        */
        void reset();
    private:
        juce::dsp::StateVariableTPTFilter<float> filter;
        bool isPrepared{ false };
        bool bypass{ true };

        enum FilterType {
            FILTER_BYPASS = 0,
            FILTER_LOWPASS,
            FILTER_BANDPASS,
            FILTER_HIGHPASS
		};
};
