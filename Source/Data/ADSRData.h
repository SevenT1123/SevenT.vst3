/*
  ==============================================================================

    ADSRData.h
    Created: 14 Jan 2025 12:41:58am
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSRData : public juce::ADSR {
    public:
        void updateADSR(const float attack, const float decay, const float sustain, const float release);

    private:
        const float minADSRTime = 0.002f; // 2 ms for ADR to prevent clicks
        juce::ADSR::Parameters adsrParameters;
};