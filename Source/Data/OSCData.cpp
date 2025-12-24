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

    // Prepare all unison oscillators
    for (auto& osc : unisonOscillators) {
        osc.prepare(spec);
    }
}

void OSCData::setWaveType(const int choice) {
    currentWaveType = choice;

    // Initialize all unison oscillators with their respective phase offsets
    for (int i = 0; i < maxUnison; ++i) {
        float unisonPhase = phaseOffset + unison.getPhaseForVoice(i);

        switch (choice) {
        case 0: // sine wave
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return std::sin(x + unisonPhase);
                });
            break;
        case 1: // saw wave
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) / juce::MathConstants<float>::pi;
                });
            break;
        case 2: // triangle wave
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (2.0f / juce::MathConstants<float>::pi) * std::asin(std::sin(x + unisonPhase));
                });
            break;
        case 3: // pulse wave
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) < 0.0f ? -1.0f : 1.0f;
                });
            break;
        case 4: // half pulse wave
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) < -juce::MathConstants<float>::pi * 0.5f ? -1.0f : 1.0f;
                });
            break;
        case 5: // quarter pulse wave
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) < -juce::MathConstants<float>::pi * 0.25f ? -1.0f : 1.0f;
                });
            break;
        case 6: // TriSaw
            unisonOscillators[i].initialise([unisonPhase](float x) {
                float p = ((x + unisonPhase) + juce::MathConstants<float>::pi) / (2.0f * juce::MathConstants<float>::pi);
                if (p < 0.5f)
                    return 4.0f * p - 1.0f;
                else
                    return 3.0f - 4.0f * p;
                });
            break;
        default:
            unisonOscillators[i].initialise([](float x) { return std::sin(x); });
            break;
        }
    }
}

void OSCData::setWaveFrequency(const int midiNoteNumber) {
    lastMidiNote = midiNoteNumber;

    // Set frequency for each unison voice with appropriate detune
    int numVoices = unison.getUnisonVoices();
    float baseFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    for (int i = 0; i < numVoices; ++i) {
        float totalDetune = cents + unison.getDetuneForVoice(i);
        float detuneRatio = std::pow(2.0f, totalDetune / 1200.0f);
        unisonOscillators[i].setFrequency(baseFreq * detuneRatio);
    }
}

void OSCData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();
    int numVoices = unison.getUnisonVoices();

    // Clear the block first
    block.clear();

    // Create a temporary buffer for each unison voice
    juce::AudioBuffer<float> tempBuffer(numChannels, numSamples);

    // Sum all unison voices
    for (int v = 0; v < numVoices; ++v) {
        tempBuffer.clear();
        juce::dsp::AudioBlock<float> tempBlock(tempBuffer);

        // Process this unison voice
        unisonOscillators[v].process(juce::dsp::ProcessContextReplacing<float>(tempBlock));

        // Get voice-specific parameters
        float voiceAmplitude = unison.getAmplitudeForVoice(v);
        float voicePan = pan + unison.getPanForVoice(v);
        voicePan = juce::jlimit(-1.0f, 1.0f, voicePan);

        // Apply volume, stereo spread, and pan for this voice
        for (int channel = 0; channel < numChannels; ++channel) {
            float panGain = 1.0f;

            if (numChannels == 2) {
                // Equal-power pan law
                float panPosition = (voicePan + 1.0f) / 2.0f; // Convert -1..1 to 0..1

                if (channel == 0) { // Left channel
                    panGain = std::cos(panPosition * juce::MathConstants<float>::halfPi);
                }
                else { // Right channel
                    panGain = std::sin(panPosition * juce::MathConstants<float>::halfPi);
                }
            }

            auto* tempData = tempBuffer.getWritePointer(channel);
            auto* blockData = block.getChannelPointer((size_t)channel);

            float finalGain = volume * voiceAmplitude * panGain;

            for (int i = 0; i < numSamples; ++i) {
                blockData[i] += tempData[i] * finalGain;
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

void OSCData::setDetune(float cents) {
    if (this->cents != cents) {
        this->cents = cents;
        if (lastMidiNote > 0) {
            setWaveFrequency(lastMidiNote);
        }
    }
}
void OSCData::setStereo(float stereo) {
    this->stereo = stereo;
}

void OSCData::setPan(float pan) {
    this->pan = pan;
}

void OSCData::setUnisonVoices(int voices) {
    unison.setUnisonVoices(voices);
    // Update frequencies for all active voices
    if (lastMidiNote > 0) {
        setWaveFrequency(lastMidiNote);
    }
}

void OSCData::setUnisonDetune(float cents) {
    unison.setUnisonDetune(cents);
    // Update frequencies for all active voices
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
