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
        float unisonPhase = phaseOffset + unison.getPhaseForVoice(i);

        switch (choice) {
        case SINE: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return std::sin(x + unisonPhase);
                });
            break;
        case SAW: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) / juce::MathConstants<float>::pi;
                });
            break;
        case TRIANGLE: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (2.0f / juce::MathConstants<float>::pi) * std::asin(std::sin(x + unisonPhase));
                });
            break;
        case PULSE: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) < 0.0f ? -1.0f : 1.0f;
                });
            break;
        case HALF_PULSE: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) < -juce::MathConstants<float>::pi * 0.5f ? -1.0f : 1.0f;
                });
            break;
        case QUARTER_PULSE: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                return (x + unisonPhase) < -juce::MathConstants<float>::pi * 0.25f ? -1.0f : 1.0f;
                });
            break;
        case TRIANGLE_SAW: 
            unisonOscillators[i].initialise([unisonPhase](float x) {
                // Normalizing period from [0, 2pi] to [0, 1]
                float p = ((x + unisonPhase) + juce::MathConstants<float>::pi) / (2.0f * juce::MathConstants<float>::pi);
                if (p < 0.5f)
                    return (x + unisonPhase) / juce::MathConstants<float>::pi;
                else
                    return (2.0f / juce::MathConstants<float>::pi) * std::asin(std::sin(x + unisonPhase - p));
                });
            break;
        case WHITE_NOISE: // Will be fixed
            unisonOscillators[i].initialise([](float x) {
                static juce::Random whiteNoise;
                return whiteNoise.nextFloat() * 2.0f - 1.0f;
                });
        default:
            unisonOscillators[i].initialise([](float x) { return std::sin(x); });
            break;
        }
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

    juce::AudioBuffer<float> tempBuffer(numChannels, numSamples);

    for (int v = 0; v < numVoices; ++v) {
        tempBuffer.clear();
        juce::dsp::AudioBlock<float> tempBlock(tempBuffer);

        unisonOscillators[v].process(juce::dsp::ProcessContextReplacing<float>(tempBlock));

        float voiceAmplitude = unison.getAmplitudeForVoice(v);
        float voicePan = pan + unison.getPanForVoice(v);
        voicePan = juce::jlimit(-1.0f, 1.0f, voicePan);

        for (int channel = 0; channel < numChannels; ++channel) {
            float panGain = 1.0f;

            if (numChannels == 2) {
                // Convert -1..1 to 0..1
                float panPosition = (voicePan + 1.0f) / 2.0f; 

                if (channel == 0) { 
                    // Left channel
                    panGain = std::cos(panPosition * juce::MathConstants<float>::halfPi);
                }
                else { 
                    // Right channel
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
