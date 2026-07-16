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
		/*
		* Constructor
		* @brief Initializes the unison data with default values and prepares phase offsets for unison voices.
		*/
		UnisonData();

		/* 
		* void prepareToPlay(juce::dsp::ProcessSpec& spec)
		* @brief: TBD
		* @params: juce::dsp::ProcessSpec& spec: TBD
		*/
		void prepareToPlay(juce::dsp::ProcessSpec& spec);

		// Setters and getters functions
		void setUnisonVoices(int voices);
		void setUnisonDetune(float cents);
		void setUnisonBlend(float blend);
		void setUnisonStereo(float stereo);


		int getUnisonVoices() const { return unisonVoices; };
		float getUnisonDetune() const { return unisonDetune; };
		float getUnisonBlend() const { return unisonBlend; };
		float getUnisonStereo() const { return unisonStereo; };

		/*
		* float getDetuneForVoice(int voiceIndex) const
		* @brief: Apply voice detune for each voice index, voice position and voice detune are distributed based on Linear Distribution model. The detune is applied symmetrically around the original frequency, and the amount of detuning is determined by the unisonDetune and unisonBlend parameters. The blend parameter allows for a more natural unison effect by applying more detuning to the outer voices and less to the inner voices.
			For example, with 4 voices and a detune of 50 cents and a blend of 0.5, the detune would be applied as follows:
			- Voice 0: -50 cents * 0.5 = -25 cents
			- Voice 1: -25 cents * 0.75 = -18.75 cents
			- Voice 2: +25 cents * 0.75 = +18.75 cents
			- Voice 3: +50 cents * 0.5 = +25 cents
			This creates a more natural unison effect by giving more prominence to the outer voices while still maintaining some detuning in the inner voices.
		* @params: int voiceIndex: Index of the unison voice [0, unisonVoices-1]
		*/
		float getDetuneForVoice(int voiceIndex) const;
		/*
		* float getPanForVoice(int voiceIndex) const
		* @brief: apply panning for each voice index, voice position and voice pan are distributed based on Linear Distribution model. The pan is applied symmetrically around the center, and the amount of panning is determined by the unisonStereo parameter. The stereo width control allows for spreading the voices across the stereo field, with 0.0 meaning all voices are centered and 1.0 meaning the voices are spread evenly across the stereo field.
			For example, with 4 voices and a stereo width of 0.5, the panning would be applied as follows:
			- Voice 0: -50% * 0.5 = -25% (left)
			- Voice 1: -25% * 0.5 = -12.5% (slightly left)
			- Voice 2: +25% * 0.5 = +12.5% (slightly right)
			- Voice 3: +50% * 0.5 = +25% (right)
			This creates a wider stereo image for the unison voices, making them sound more spacious and less centered.
		* @params: int voiceIndex: Index of the unison voice [0, unisonVoices-1]
		*/
		float getPanForVoice(int voiceIndex) const;
		/*
		* float getPhaseForVoice(int voiceIndex) const
		* @brief: Apply phase offset for voice index.
		* @param: int voiceIndex: Index of the unison voice [0, unisonVoices-1]
		*/
		float getPhaseForVoice(int voiceIndex) const;
		/* 
		* float getAmplitudeForVoice(int voiceIndex) const
		* @brief: Apply amplitude scaling for voice index. The amplitude is normalized so that the total energy of all voices remains constant, which means that each voice's amplitude is reduced based on the number of unison voices. This helps to prevent the overall sound from becoming too loud as more voices are added. Additionally, a slight amplitude variation can be applied to each voice to create a more organic and natural sound, avoiding a static and artificial feel.
		* @param: int voiceIndex: Index of the unison voice [0, unisonVoices-1]
		*/
		float getAmplitudeForVoice(int voiceIndex) const;
    private:
		static constexpr int maxUnison = 16;

		/* @params
		* int unisonVoices: Number of unison voices [1,maxUnison]
		* float unisonDetune: Amount of detuning in cents [0.0, 100.0]. Determines how much unison voices are from original frequency. 0.0 means no detuning, 100.0 means maximum detuning. Detune is applied symmetrically around the original frequency. 
			For example, with 4 voices and 50 cents detune, the voices would be at -50 cents, -25 cents, +25 cents, and +50 cents from the original frequency.
		* float unisonBlend: Amount of blend between detuned and original frequencies [0.0, 1.0]. Determines how the detune is applied across the unison voices. 0.0 means only the outer voices are detuned, while 1.0 means all voices are equally detuned. 
			For example, with 4 voices and a blend of 0.5, the detune would be applied more to the outer voices and less to the inner voices, creating a more natural unison effect.
		* float unisonStereo: Amount of stereo width for unison voices [0.0, 1.0]. Determines how the unison voices are panned across the stereo field. 0.0 means all voices are centered, while 1.0 means the voices are spread evenly across the stereo field. 
			For example, with 4 voices and a stereo width of 0.5, the voices would be panned at -50%, -25%, +25%, and +50% across the stereo field.
		* std::array<float, maxUnison> phaseOffsets: Pre-calculated random phase offsets for each unison voice to create a thicker sound. These offsets are applied to the phase of each voice to ensure that they do not all start at the same point, which can help to reduce phasing issues and create a richer sound. The offsets are generated randomly when the UnisonData object is created and can be updated if needed.
			For example, with 4 voices, you might have random phase offsets of 0.0, 90.0, 180.0, and 270.0 degrees for each voice, which would help to create a more complex and interesting sound when the voices are combined.
		* juce::Random random: Generate random phase offsets for each unison voice.
		*/  
		int unisonVoices = 1;
		float unisonDetune = 0.0f; 
		float unisonBlend = 0.0f;  
		float unisonStereo = 0.0f; 
		std::array<float, maxUnison> phaseOffsets;
		juce::Random random;

		/* 
		* void updatePhaseOffsets()
		* @brief: Generate random phase offsets for each unison voice.
		*/
		void updatePhaseOffsets();
};