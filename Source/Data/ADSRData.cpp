/*
  ==============================================================================

    ADSRData.cpp
    Created: 14 Jan 2025 12:41:58am
    Author:  Seven T

  ==============================================================================
*/

#include "ADSRData.h"

void ADSRData::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    adsrParameters.attack = juce::jmax(attack, maxADSRTime);
    adsrParameters.decay = juce::jmax(decay, maxADSRTime);
    adsrParameters.sustain = sustain;
    adsrParameters.release = juce::jmax(release, maxADSRTime);

    setParameters(adsrParameters);
}
