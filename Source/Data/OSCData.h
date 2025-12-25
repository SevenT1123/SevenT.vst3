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
        void prepareToPlay(juce::dsp::ProcessSpec& spec);
        void setWaveType(const int choice);
        void setWaveFrequency(const int midiNoteNumber);
        void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

        void setVolume(float volume);
        void setPhaseOffset(float phaseOffset); 
        void setPan(float pan);

        float getVolume() const { return volume; };
        float getPhaseOffset() const { return phaseOffset; };;
        float getPan() const { return pan; };

        void setUnisonVoices(int voices);
        void setUnisonDetune(float cents);
        void setUnisonBlend(float blend);
        void setUnisonStereo(float stereo);
        UnisonData& getUnison() { return unison; };
    private:
        int lastMidiNote{ 0 };
		static constexpr int maxUnison = 16;

        float volume{ 0.0f }; // 0.0 silent, 1.0 full
        float phaseOffset{ 0.0f }; // in radians
        float pan{ 0.0f }; // -1.0 left, 0.0 center, +1.0 right

        UnisonData unison;
        std::array<juce::dsp::Oscillator<float>, 16> unisonOscillators;
        int currentWaveType{ 0 };
        juce::dsp::ProcessSpec currentSpec;
};
