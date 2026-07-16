/*
  ==============================================================================

    SynthVoice.h
    Created: 8 Jan 2025 6:20:01pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <atomic>
#include "SynthSound.h"
#include "Data/ADSRData.h"
#include "Data/OSCData.h"
#include "Data/FilterData.h"

class SynthVoice : public juce::SynthesiserVoice {
    public:
        bool canPlaySound(juce::SynthesiserSound* sound) override;
        void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
        void stopNote(float velocity, bool allowTailOff) override;
        void controllerMoved(int controllerNumber, int newControllerValue) override;
        void pitchWheelMoved(int newPitchWheelValue) override;
        void prepareToPlay(double sampleRate, int samplePerBlock, int outputChannels);
        void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
        void update(const float attack, const float decay, const float sustain, const float release);
		// Getter functions for the oscillator, ADSR, and filter data
        OSCData& getOscillator() { return osc; }
        OSCData& getOscillator2() { return osc2; }
        ADSRData& getAdsr() { return adsr; }
        FilterData& getFilter() { return filter; }
    /* void setVoiceIndex(int index)
    * @brief: Tags this voice with its position (0-based) in the Synthesiser's voice array.
    *         Used together with the shared polyphony limit to decide whether this voice
    *         is allowed to take new notes.
    */
    void setVoiceIndex(int index) { voiceIndex = index; }

    /* void setPolyphonyLimit(std::atomic<int>* limit)
    * @brief: Gives the voice a pointer to the processor-owned atomic polyphony limit so
    *         canPlaySound() can check it without any locking or allocation on the audio thread.
    *         The pointer is expected to outlive the voice (owned by SevenTAudioProcessor).
    */
    void setPolyphonyLimit(std::atomic<int>* limit) { polyphonyLimit = limit; }

    private:
        juce::AudioBuffer<float> synthBuffer;
        juce::AudioBuffer<float> osc2Buffer;

        OSCData osc;
        OSCData osc2;   
        ADSRData adsr;
        FilterData filter;
        juce::dsp::Gain<float> gain;

        bool isPrepared{ false };

        int voiceIndex{ 0 };
        std::atomic<int>* polyphonyLimit{ nullptr };
};