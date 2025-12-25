/*
  ==============================================================================

    FilterComponent.cpp
    Created: 23 Jan 2025 8:15:54pm
    Author:  Seven T

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorID, juce::String filterFreqID, juce::String filterResID)
{
    juce::StringArray choices{ "Bypass", "Low-Pass", "Band-Pass", "High-Pass" };
    filterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(filterTypeSelector);

    filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorID, filterTypeSelector);
    
    setSliderWithLabel(filterFreqSlider, filterFreqLabel, apvts, filterFreqID, filterFreqAttachment);
    setSliderWithLabel(filterResSlider, filterResLabel, apvts, filterResID, filterResAttachment);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(5);
    auto labelSpace = bounds.removeFromTop(25.0f);

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Filter", labelSpace.withX(5), juce::Justification::left);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void FilterComponent::resized()
{
    const auto startY = 40;
    const auto padding = 10;
    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    filterTypeSelector.setBounds(10, startY, 90, 30);

    const auto sliderPosY = filterTypeSelector.getBottom();

    filterFreqSlider.setBounds(10, sliderPosY + labelYOffset + padding, 90, sliderHeight);
    filterFreqLabel.setBounds(filterFreqSlider.getX(), filterFreqSlider.getY() - labelYOffset, filterFreqSlider.getWidth(), labelHeight);

    filterResSlider.setBounds(filterFreqSlider.getRight(), sliderPosY + labelYOffset + padding, sliderWidth, sliderHeight);
    filterResLabel.setBounds(filterResSlider.getX(), filterResSlider.getY() - labelYOffset, filterResSlider.getWidth(), labelHeight);
}

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment) {
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}