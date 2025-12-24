/*
  ==============================================================================

    UnisonData.cpp
    Created: 21 Dec 2025 10:21:51pm
    Author:  Seven T

  ==============================================================================
*/

#include "UnisonData.h"

UnisonData::UnisonData() {
    updatePhaseOffsets();
}

void UnisonData::prepareToPlay(juce::dsp::ProcessSpec& spec) {
    // Initialize any DSP components if needed
}

void UnisonData::setUnisonVoices(int voices) {
    unisonVoices = juce::jlimit(1, 16, voices);
}

void UnisonData::setUnisonDetune(float cents) {
    unisonDetune = juce::jlimit(0.0f, 100.0f, cents);
}

void UnisonData::setUnisonBlend(float blend) {
    unisonBlend = juce::jlimit(0.0f, 1.0f, blend);
}

void UnisonData::setUnisonStereo(float stereo) {
    unisonStereo = juce::jlimit(0.0f, 1.0f, stereo);
}

float UnisonData::getDetuneForVoice(int voiceIndex) const {
    if (unisonVoices <= 1)
        return 0.0f;

    // Distribute voices evenly from -detune to +detune
    float normalizedPosition = (voiceIndex - (unisonVoices - 1) * 0.5f) / (unisonVoices - 1);

    // Apply detune with blend control
    // When blend = 0, only outer voices are detuned
    // When blend = 1, all voices are equally distributed
    float blendFactor = 1.0f - unisonBlend * (1.0f - std::abs(normalizedPosition));

    return normalizedPosition * unisonDetune * 2.0f * blendFactor;
}

float UnisonData::getPanForVoice(int voiceIndex) const {
    if (unisonVoices <= 1)
        return 0.0f;

    // Distribute voices across stereo field
    float normalizedPosition = (voiceIndex - (unisonVoices - 1) * 0.5f) / (unisonVoices - 1);

    // Apply stereo width control
    return normalizedPosition * unisonStereo;
}

float UnisonData::getPhaseForVoice(int voiceIndex) const {
    if (voiceIndex >= 0 && voiceIndex < maxUnison)
        return phaseOffsets[voiceIndex];
    return 0.0f;
}

float UnisonData::getAmplitudeForVoice(int voiceIndex) const {
    if (unisonVoices <= 1)
        return 1.0f;

    // Normalize amplitude so total energy remains constant
    // Each voice gets reduced amplitude based on number of voices
    float baseAmplitude = 1.0f / std::sqrt(static_cast<float>(unisonVoices));

    // Optional: Apply slight amplitude variation for more organic sound
    float normalizedPosition = std::abs((voiceIndex - (unisonVoices - 1) * 0.5f) / (unisonVoices - 1));
    float variation = 1.0f - (normalizedPosition * 0.1f); // 10% max variation

    return baseAmplitude * variation;
}

void UnisonData::updatePhaseOffsets() {
    // Generate random phase offsets for each possible unison voice
    juce::Random random;
    for (int i = 0; i < maxUnison; ++i) {
        phaseOffsets[i] = random.nextFloat() * juce::MathConstants<float>::twoPi;
    }
}