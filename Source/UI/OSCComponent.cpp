/*
  ==============================================================================

    OSCComponent.cpp
    Created: 16 Jan 2025 2:32:55pm
    Author:  Seven T

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OSCComponent.h"

//==============================================================================
OSCComponent::OSCComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, 
    juce::String oscVolumeId,
    juce::String oscPhaseOffsetId,
    juce::String oscPanId, 
    juce::String unisonVoicesID,
    juce::String unisonDetuneID,
    juce::String unisonBlendID,
    juce::String unisonStereoID)
{
    juce::StringArray choices{ "Sine", "Saw", "Triangle", "Pulse", "H-Pulse", "Q-Pulse", "TriSaw"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorID, oscWaveSelector);    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);

	setSliderWithLabel(oscVolumeSlider, oscVolumeLabel, apvts, oscVolumeId, oscVolumeAttachment);
	setSliderWithLabel(oscPhaseOffsetSlider, oscPhaseOffsetLabel, apvts, oscPhaseOffsetId, oscPhaseOffsetAttachment);
	setSliderWithLabel(oscPanSlider, oscPanLabel, apvts, oscPanId, oscPanAttachment);

    // Unison parameters
    setSliderWithLabel(unisonVoicesSlider, unisonVoicesLabel, apvts, unisonVoicesID, unisonVoicesAttachment);
    setSliderWithLabel(unisonDetuneSlider, unisonDetuneLabel, apvts, unisonDetuneID, unisonDetuneAttachment);
    setSliderWithLabel(unisonBlendSlider, unisonBlendLabel, apvts, unisonBlendID, unisonBlendAttachment);
    setSliderWithLabel(unisonStereoSlider, unisonStereoLabel, apvts, unisonStereoID, unisonStereoAttachment);

    unisonVoicesSlider.setNumDecimalPlacesToDisplay(0);
}

OSCComponent::~OSCComponent()
{
}

void OSCComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Oscillator", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void OSCComponent::resized()
{
    const auto startY = 60;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    oscWaveSelector.setBounds(10, startY + 10, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);

	oscVolumeSlider.setBounds(oscWaveSelector.getRight(), startY, sliderWidth, sliderHeight);
	oscVolumeLabel.setBounds(oscVolumeSlider.getX(), oscVolumeSlider.getY() - labelYOffset, oscVolumeSlider.getWidth(), labelHeight);

	oscPhaseOffsetSlider.setBounds(oscVolumeSlider.getRight(), startY, sliderWidth, sliderHeight);
	oscPhaseOffsetLabel.setBounds(oscPhaseOffsetSlider.getX(), oscPhaseOffsetSlider.getY() - labelYOffset, oscPhaseOffsetSlider.getWidth(), labelHeight);

    oscPanSlider.setBounds(oscPhaseOffsetSlider.getRight(), startY, sliderWidth, sliderHeight);
    oscPanLabel.setBounds(oscPanSlider.getX(), oscPanSlider.getY() - labelYOffset, oscPanSlider.getWidth(), labelHeight);

    // Bottom row: Unison controls
    const auto secondRowY = oscVolumeSlider.getBottom() + 30;
    const auto unisonLabelY = secondRowY - 25;

    unisonVoicesSlider.setBounds(5, secondRowY, sliderWidth, sliderHeight);
    unisonVoicesLabel.setBounds(unisonVoicesSlider.getX(), unisonVoicesSlider.getY() - labelYOffset, unisonVoicesSlider.getWidth(), labelHeight);

    unisonDetuneSlider.setBounds(unisonVoicesSlider.getRight(), secondRowY, sliderWidth, sliderHeight);
    unisonDetuneLabel.setBounds(unisonDetuneSlider.getX(), unisonDetuneSlider.getY() - labelYOffset, unisonDetuneSlider.getWidth(), labelHeight);

    unisonBlendSlider.setBounds(unisonDetuneSlider.getRight(), secondRowY, sliderWidth, sliderHeight);
    unisonBlendLabel.setBounds(unisonBlendSlider.getX(), unisonBlendSlider.getY() - labelYOffset, unisonBlendSlider.getWidth(), labelHeight);

    unisonStereoSlider.setBounds(unisonBlendSlider.getRight(), secondRowY, sliderWidth, sliderHeight);
    unisonStereoLabel.setBounds(unisonStereoSlider.getX(), unisonStereoSlider.getY() - labelYOffset, unisonStereoSlider.getWidth(), labelHeight);
}

void OSCComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
    