/*
  ==============================================================================

    UnisonData.h
    Created: 21 Dec 2025 10:21:51pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class UnisonData : public juce::dsp::Oscillator<float> {
    public:
		UnisonData();

		void prepareToPlay(juce::dsp::ProcessSpec& spec);
		void setUnisonVoices(int voices);
		void setUnisonDetune(float cents);
		void setUnisonBlend(float blend);
		void setUnisonStereo(float stereo);

		int getUnisonVoices() const { return unisonVoices; };
		float getUnisonDetune() const { return unisonDetune; };
		float getUnisonBlend() const { return unisonBlend; };
		float getUnisonStereo() const { return unisonStereo; };

		float getDetuneForVoice(int voiceIndex) const;
		float getPanForVoice(int voiceIndex) const;
		float getPhaseForVoice(int voiceIndex) const;
		float getAmplitudeForVoice(int voiceIndex) const;
    private:
		static constexpr int maxUnison = 16;

		int unisonVoices = 1;
		float unisonDetune = 0.0f; // 0.0 to 100.0 
		float unisonBlend = 0.0f;  // 0.0 to 1.0
		float unisonStereo = 0.0f; // 0.0 to 1.0

		std::array<float, maxUnison> phaseOffsets;

		void updatePhaseOffsets();
};