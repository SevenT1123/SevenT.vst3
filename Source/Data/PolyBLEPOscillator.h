//
// Created by Seven T on 7/8/2026.
//

#pragma once

#include <JuceHeader.h>

/*
 * class PolyBLEPOscillator
 * @brief: A band-limited oscillator using the PolyBLEP (Polynomial Band-Limited Step) technique
 *         to reduce aliasing on waveforms that contain discontinuities (saw, pulse/square).
 *         One instance is used per unison voice inside OSCData.
 */
class PolyBLEPOscillator {
    public:
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

        /*
        * void prepare(const juce::dsp::ProcessSpec& spec)
        * @brief: Stores the sample rate needed to convert frequency (Hz) into a phase increment (dt).
        * @param: const juce::dsp::ProcessSpec& spec: processing spec containing the sample rate.
        */
        void prepare(const juce::dsp::ProcessSpec& spec);

        /*
        * void setWaveType(WaveType type)
        * @brief: Sets which waveform getNextSample() will generate.
        * @param: WaveType type: the desired waveform.
        */
        void setWaveType(WaveType type);

        /*
        * void setFrequency(float freq)
        * @brief: Sets the oscillator frequency in Hz and recalculates the phase increment using the
        *         sample rate captured in prepare().
        * @param: float freq: frequency in Hz.
        */
        void setFrequency(float freq);

        /*
        * void setPhaseOffset(float radians)
        * @brief: Offsets the oscillator's phase, used to de-phase unison voices so they don't all
        *         start perfectly in phase. Input is in radians (matches OSCData's [-pi, pi] convention,
        *         though any value is accepted and wrapped) and is converted internally to a normalized
        *         [0, 1) phase fraction.
        * @param: float radians: phase offset in radians.
        */
        void setPhaseOffset(float radians);

        /*
        * void reset()
        * @brief: Resets the running phase back to 0. Does not affect frequency, wave type, or phase offset.
        */
        void reset();

        /*
        * float getNextSample()
        * @brief: Generates and returns the next output sample for the current wave type, then advances phase.
        */
        float getNextSample();

    private:
        float polyBLEP(float t, float dt) const;
        float generatePulse(float p, float dt, float dutyCycle) const;

        double sampleRate{ 44100.0 };
        float frequency{ 440.0f };
        float phase{ 0.0f };
        float phaseOffset{ 0.0f };
        float dt{ 0.0f };
        WaveType waveType{ SAW };

        juce::Random random;
};