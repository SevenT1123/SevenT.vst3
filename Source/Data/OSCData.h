/*
  ==============================================================================

    OSCData.h
    Created: 16 Jan 2025 1:56:01pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "UnisonData.h"

class OSCData : juce::dsp::Oscillator<float> {
    public:
        /*
        * void prepareToPlay(juce::dsp::ProcessSpec& spec)
		* @brief: Prepares the oscillator for playback by initializing it with the given processing specifications.
		* @param: juce::dsp::ProcessSpec& spec: A reference to a ProcessSpec object containing the sample rate, maximum block size, and number of channels.
        */
        void prepareToPlay(juce::dsp::ProcessSpec& spec);
        /*
        * void setWaveType(const int choice)
		* @brief: Set synthesizer wave type based on user choice and initialize the unison oscillators with the corresponding waveform function. The function initializes each unison oscillator with a lambda function from juce::dsp::Oscillator that generates the appropriate waveform based on the current phase offset and unison settings.
		* @params: const int choice: An integer representing the selected wave type (e.g., 0 for sine, 1 for saw, etc.).
        */
        void setWaveType(const int choice);
        /* 
        * void setWaveFrequency(const int midiNoteNumber)
		* @brief: Set the frequency of the oscillator based on the MIDI note number and update the frequencies of all unison voices accordingly. The function calculates the base frequency using juce::MidiMessage::getMidiNoteInHertz and applies detuning for each unison voice based on the unison settings, ensuring that all voices are correctly tuned to produce a cohesive sound.
		* @params: const int midiNoteNumber: An integer representing the MIDI note number for which to set the frequency (A4 = 440Hz).
        */
        void setWaveFrequency(const int midiNoteNumber);
        /* 
        * void getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
		* @brief: Generates the next block of audio samples for the oscillator, applying volume, pan, and unison effects. The function processes each unison voice through its respective oscillator, applies amplitude and panning based on the unison settings, and mixes the results into the output audio block. This allows for rich, layered sounds with control over individual voice characteristics.
		* @param: juce::dsp::AudioBlock<float>& block: A reference to an AudioBlock object where the generated audio samples will be stored.
        */
        void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

		// Setter and getter functions
        void setVolume(float volume);
        void setPhaseOffset(float phaseOffset); 
        void setPan(float pan);

        float getVolume() const { return volume; };
        float getPhaseOffset() const { return phaseOffset; };
        float getPan() const { return pan; };

        void setUnisonVoices(int voices);
        void setUnisonDetune(float cents);
        void setUnisonBlend(float blend);
        void setUnisonStereo(float stereo);
        UnisonData& getUnison() { return unison; };
    private:
        int lastMidiNote{ 0 };
		static const int maxUnison = 16;

        // @params: float volume in [0.0, 1.0], float phaseOffset in [-pi, pi], float pan in [-1.0, 1.0] where -1.0 is full left and 1.0 is full right.
        float volume{ 0.0f }; 
        float phaseOffset{ 0.0f }; 
        float pan{ 0.0f }; 

        enum WaveType {
            SINE = 0,
            SAW,
            TRIANGLE,
            PULSE,
            HALF_PULSE,
            QUARTER_PULSE,
            TRIANGLE_SAW,
			WHITE_NOISE
		};

        UnisonData unison;
        std::array<juce::dsp::Oscillator<float>, 16> unisonOscillators;
        int currentWaveType{ 0 };
        juce::dsp::ProcessSpec currentSpec;
};
