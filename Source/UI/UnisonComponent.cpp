/*
  ==============================================================================

    UnisonComponent.cpp
    Created: 22 Dec 2025 6:48:46pm
    Author:  Seven T

  ==============================================================================
*/

#include <JuceHeader.h>
#include "UnisonComponent.h"

//==============================================================================
UnisonComponent::UnisonComponent(juce::AudioProcessorValueTreeState& apvts,
    juce::String unisonVoicesId,
    juce::String unisonDetuneId,
    juce::String unisonBlendId,
    juce::String unisonStereoId)
{
    setSliderWithLabel(unisonVoicesSlider, unisonVoicesLabel, apvts, unisonVoicesId, unisonVoicesAttachment);
    setSliderWithLabel(unisonDetuneSlider, unisonDetuneLabel, apvts, unisonDetuneId, unisonDetuneAttachment);
    setSliderWithLabel(unisonBlendSlider, unisonBlendLabel, apvts, unisonBlendId, unisonBlendAttachment);
    setSliderWithLabel(unisonStereoSlider, unisonStereoLabel, apvts, unisonStereoId, unisonStereoAttachment);

    // Set voices slider to show integer values
    unisonVoicesSlider.setNumDecimalPlacesToDisplay(0);
}

UnisonComponent::~UnisonComponent()
{
}

void UnisonComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Unison", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void UnisonComponent::resized()
{
    const auto startY = 55;
    const auto sliderWidth = 70;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;
    const auto padding = 5;

    unisonVoicesSlider.setBounds(10, startY, sliderWidth, sliderHeight);
    unisonVoicesLabel.setBounds(unisonVoicesSlider.getX(), startY - labelYOffset, sliderWidth, labelHeight);

    unisonDetuneSlider.setBounds(unisonVoicesSlider.getRight() + padding, startY, sliderWidth, sliderHeight);
    unisonDetuneLabel.setBounds(unisonDetuneSlider.getX(), startY - labelYOffset, sliderWidth, labelHeight);

    unisonBlendSlider.setBounds(unisonDetuneSlider.getRight() + padding, startY, sliderWidth, sliderHeight);
    unisonBlendLabel.setBounds(unisonBlendSlider.getX(), startY - labelYOffset, sliderWidth, labelHeight);

    unisonStereoSlider.setBounds(unisonBlendSlider.getRight() + padding, startY, sliderWidth, sliderHeight);
    unisonStereoLabel.setBounds(unisonStereoSlider.getX(), startY - labelYOffset, sliderWidth, labelHeight);
}

void UnisonComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}
