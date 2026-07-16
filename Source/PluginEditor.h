/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/OSCComponent.h"
#include "UI/FilterComponent.h"
#include "UI/PresetComponent.h"


//==============================================================================
/**
*/
class SevenTAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SevenTAudioProcessorEditor (SevenTAudioProcessor&);
    ~SevenTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SevenTAudioProcessor& audioProcessor;
    PresetComponent presetComponent;
    OSCComponent osc;
    OSCComponent osc2;
    ADSRComponent adsr;
    FilterComponent filter;

    static constexpr int width = 1024;
    static constexpr int height = 768;
    
    juce::Image logo;
	void loadLogoImage();

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    juce::Slider polyphonySlider;
    juce::Label polyphonyLabel{ "Polyphony", "Polyphony" };
    std::unique_ptr<Attachment> polyphonyAttachment;

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SevenTAudioProcessorEditor)
};
