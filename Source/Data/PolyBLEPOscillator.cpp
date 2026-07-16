//
// Created by Seven T on 7/8/2026.
//

#include "PolyBLEPOscillator.h"

void PolyBLEPOscillator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    if (sampleRate > 0.0)
        dt = frequency / (float)sampleRate;
}

void PolyBLEPOscillator::setWaveType(WaveType type)
{
    waveType = type;
}

void PolyBLEPOscillator::setFrequency(float freq)
{
    frequency = freq;

    if (sampleRate > 0.0)
        dt = frequency / (float)sampleRate;
}

void PolyBLEPOscillator::setPhaseOffset(float radians)
{
    phaseOffset = radians / juce::MathConstants<float>::twoPi;
    phaseOffset -= std::floor(phaseOffset); // wrap into [0, 1)
}

void PolyBLEPOscillator::reset()
{
    phase = 0.0f;
}

float PolyBLEPOscillator::polyBLEP(float t, float dt) const
{
    if (dt <= 0.0f)
        return 0.0f;

    if (t < dt)
    {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    else if (t > 1.0f - dt)
    {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }

    return 0.0f;
}

float PolyBLEPOscillator::generatePulse(float p, float dt, float dutyCycle) const
{
    float value = p < dutyCycle ? 1.0f : -1.0f;

    // correct the rising edge at p == 0
    value += polyBLEP(p, dt);
    // correct the falling edge at p == dutyCycle
    value -= polyBLEP(std::fmod(p + 1.0f - dutyCycle, 1.0f), dt);

    return value;
}

float PolyBLEPOscillator::getNextSample()
{
    float p = phase + phaseOffset;
    p -= std::floor(p);

    float value = 0.0f;

    switch (waveType)
    {
        case SINE:
        {
            value = std::sin(juce::MathConstants<float>::twoPi * p);
            break;
        }
        case SAW:
        {
            value = 2.0f * p - 1.0f - polyBLEP(p, dt);
            break;
        }
        case TRIANGLE:
        {
            // integral of a square wave, continuous so no BLEP correction needed
            value = 2.0f * std::fabs(2.0f * p - 1.0f) - 1.0f;
            break;
        }
        case PULSE:
        {
            value = generatePulse(p, dt, 0.5f);
            break;
        }
        case HALF_PULSE:
        {
            value = generatePulse(p, dt, 0.25f);
            break;
        }
        case QUARTER_PULSE:
        {
            value = generatePulse(p, dt, 0.125f);
            break;
        }
        case TRIANGLE_SAW:
        {
            // asymmetric triangle: rises for 90% of the cycle, falls for 10%,
            // giving a shape between a triangle and a sawtooth
            const float skew = 0.9f;

            if (p < skew)
                value = (2.0f * p / skew) - 1.0f;
            else
                value = 1.0f - (2.0f * (p - skew) / (1.0f - skew));

            break;
        }
        case WHITE_NOISE:
        {
            value = random.nextFloat() * 2.0f - 1.0f;
            break;
        }
        default:
            break;
    }

    phase += dt;
    if (phase >= 1.0f)
        phase -= 1.0f;

    return value;
}
