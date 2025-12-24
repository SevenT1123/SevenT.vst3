/*
  ==============================================================================

    UnisonComponent.h
    Created: 22 Dec 2025 6:48:46pm
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class UnisonComponent  : public juce::Component
{
public:
    UnisonComponent(juce::AudioProcessorValueTreeState& apvts,
        juce::String unisonVoicesId,
        juce::String unisonDetuneId,
        juce::String unisonBlendId,
        juce::String unisonStereoId);
    ~UnisonComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider unisonVoicesSlider;
    juce::Slider unisonDetuneSlider;
    juce::Slider unisonBlendSlider;
    juce::Slider unisonStereoSlider;

    std::unique_ptr<Attachment> unisonVoicesAttachment;
    std::unique_ptr<Attachment> unisonDetuneAttachment;
    std::unique_ptr<Attachment> unisonBlendAttachment;
    std::unique_ptr<Attachment> unisonStereoAttachment;

    juce::Label unisonVoicesLabel{ "Voices", "Voices" };
    juce::Label unisonDetuneLabel{ "Detune", "Detune" };
    juce::Label unisonBlendLabel{ "Blend", "Blend" };
    juce::Label unisonStereoLabel{ "Stereo", "Stereo" };

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnisonComponent)
};
