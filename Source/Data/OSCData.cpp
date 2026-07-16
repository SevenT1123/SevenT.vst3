/*
  ==============================================================================

    OSCData.cpp
    Created: 16 Jan 2025 1:56:01pm
    Author:  Seven T

  ==============================================================================
*/

#include "OSCData.h"

void OSCData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    currentSpec = spec;
    unison.prepareToPlay(spec);

    for (auto& osc : unisonOscillators) {
        osc.prepare(spec);
    }
}

void OSCData::setWaveType(const int choice) {
    currentWaveType = choice;

    for (int i = 0; i < maxUnison; ++i) {
        unisonOscillators[i].setWaveType(static_cast<PolyBLEPOscillator::WaveType>(choice));
        unisonOscillators[i].setPhaseOffset(phaseOffset + unison.getPhaseForVoice(i));
    }
}

void OSCData::setWaveFrequency(const int midiNoteNumber) {
    lastMidiNote = midiNoteNumber;

    int numVoices = unison.getUnisonVoices();
    float baseFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    for (int i = 0; i < numVoices; ++i) {
        float detuneRatio = std::pow(2.0f, unison.getDetuneForVoice(i) / 1200.0f);
        unisonOscillators[i].setFrequency(baseFreq * detuneRatio);
    }
}

void OSCData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();
    int numVoices = unison.getUnisonVoices();

    block.clear();

    for (int v = 0; v < numVoices; ++v) {
        float voiceAmplitude = unison.getAmplitudeForVoice(v);
        float voicePan = juce::jlimit(-1.0f, 1.0f, pan + unison.getPanForVoice(v));
        float panPosition = (voicePan + 1.0f) / 2.0f;
        float leftGain = std::cos(panPosition * juce::MathConstants<float>::halfPi);
        float rightGain = std::sin(panPosition * juce::MathConstants<float>::halfPi);
        float finalGain = volume * voiceAmplitude;

        for (size_t i = 0; i < numSamples; ++i) {
            float sample = unisonOscillators[v].getNextSample() * finalGain;

            for (size_t channel = 0; channel < numChannels; ++channel) {
                float panGain = (numChannels == 2) ? (channel == 0 ? leftGain : rightGain) : 1.0f;
                block.getChannelPointer(channel)[i] += sample * panGain;
            }
        }
    }
}

void OSCData::setVolume(float volume) {
	this->volume = volume;
}

void OSCData::setPhaseOffset(float phaseOffset) {
    if (this->phaseOffset != phaseOffset) {
        this->phaseOffset = phaseOffset;
        setWaveType(currentWaveType);
    }
}

void OSCData::setPan(float pan) {
    this->pan = pan;
}

void OSCData::setUnisonVoices(int voices) {
    unison.setUnisonVoices(voices);
    if (lastMidiNote > 0) {
        setWaveFrequency(lastMidiNote);
    }
}

void OSCData::setUnisonDetune(float cents) {
    unison.setUnisonDetune(cents);
    if (lastMidiNote > 0) {
        setWaveFrequency(lastMidiNote);
    }
}

void OSCData::setUnisonBlend(float blend) {
    unison.setUnisonBlend(blend);
}

void OSCData::setUnisonStereo(float stereo) {
    unison.setUnisonStereo(stereo);
}
