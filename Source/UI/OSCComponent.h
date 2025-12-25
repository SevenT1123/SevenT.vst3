/*
  ==============================================================================

    OSCComponent.h
    Created: 16 Jan 2025 2:32:55pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OSCComponent  : public juce::Component
{
public:
    OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, 
        juce::String oscVolumeId, 
        juce::String oscPhaseOffsetId,  
        juce::String oscPanId, 
        juce::String unisonVoicesID,
        juce::String unisonDetuneID,
        juce::String unisonBlendID,
        juce::String unisonStereoID);
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

    juce::Slider oscVolumeSlider;
    juce::Slider oscPhaseOffsetSlider;
    juce::Slider oscPanSlider;

    juce::Slider unisonVoicesSlider;
    juce::Slider unisonDetuneSlider;
    juce::Slider unisonBlendSlider;
    juce::Slider unisonStereoSlider;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Attachment> oscVolumeAttachment;
    std::unique_ptr<Attachment> oscPhaseOffsetAttachment;
    std::unique_ptr<Attachment> oscPanAttachment;

    std::unique_ptr<Attachment> unisonVoicesAttachment;
    std::unique_ptr<Attachment> unisonDetuneAttachment;
    std::unique_ptr<Attachment> unisonBlendAttachment;
    std::unique_ptr<Attachment> unisonStereoAttachment;

    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };

    juce::Label oscVolumeLabel{ "Volume", "Volume" };
    juce::Label oscPhaseOffsetLabel{ "Phase Offset", "Phase Offset" };
    juce::Label oscPanLabel{ "Pan", "Pan" };

    juce::Label unisonVoicesLabel{ "Voices", "Voices" };
    juce::Label unisonDetuneLabel{ "Detune", "Detune" };
    juce::Label unisonBlendLabel{ "Blend", "Blend" };
    juce::Label unisonStereoLabel{ "Width", "Width" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
