/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SevenTAudioProcessorEditor::SevenTAudioProcessorEditor (SevenTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), presetComponent(p.getPresetLoad())
    , osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1VOLUME", "OSC1PHASEOFFSET", "OSC1PAN", "OSC1UNISONVOICES", "OSC1UNISONDETUNE", "OSC1UNISONBLEND", "OSC1UNISONSTEREO")
    , osc2(audioProcessor.apvts, "OSC2WAVETYPE", "OSC2VOLUME", "OSC2PHASEOFFSET", "OSC2PAN", "OSC2UNISONVOICES", "OSC2UNISONDETUNE", "OSC2UNISONBLEND", "OSC2UNISONSTEREO")
    , adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , filter(audioProcessor.apvts, "FILTERTYPE", "FILTERFREQ", "FILTERRES")
{
    setSize(width, height);
    addAndMakeVisible(presetComponent);
    addAndMakeVisible(osc);
    addAndMakeVisible(osc2);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);

    setSliderWithLabel(polyphonySlider, polyphonyLabel, audioProcessor.apvts, "POLYPHONY", polyphonyAttachment);

    loadLogoImage();
}

SevenTAudioProcessorEditor::~SevenTAudioProcessorEditor()
{
}

//==============================================================================
void SevenTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    if (logo.isValid())
    {
        const int logoWidth = 100;  // Adjust size as needed
        const int logoHeight = 100; // Adjust size as needed
        const int padding = 10;

        // Position in top right corner
        juce::Rectangle<int> logoBounds(getWidth() - logoWidth - padding, padding, logoWidth, logoHeight); // (x pos, y pos, width, height)

        // Draw the image with transparency preserved
        g.drawImage(logo, logoBounds.toFloat(),
            juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize);
    }
}

void SevenTAudioProcessorEditor::resized()
{
    const auto paddingX = 5;
    const auto paddingY = 50;
    const auto paddingY2 = paddingY + 350;
    const int presetPanelWidth = 500;
    const auto presetPanelHeight = 40;
    const int presetPanelX = (getWidth() - presetPanelWidth) / 2;
    const int presetPanelY = 10;

    const auto sliderWidth = 100;
    const auto sliderHeight = 90;
    const auto labelYOffset = 20;
    const auto labelHeight = 20;

    presetComponent.setBounds(presetPanelX, presetPanelY, presetPanelWidth, presetPanelHeight);

    osc.setBounds(paddingX, paddingY, 405, 300);
    osc2.setBounds(osc.getRight(), paddingY, 405, 300);
    adsr.setBounds(osc.getRight(), paddingY2, 250, 200);
    filter.setBounds(paddingX, paddingY2, 300, 200);
    polyphonySlider.setBounds(width - sliderWidth - paddingX, height - sliderHeight - labelHeight, sliderWidth, sliderHeight);
    polyphonyLabel.setBounds(polyphonySlider.getX(), polyphonySlider.getY() - labelYOffset, polyphonySlider.getWidth(), labelHeight);
}

void SevenTAudioProcessorEditor::loadLogoImage() {
    int dataSize = 0;
	const char* imageData = BinaryData::sevent_blackwhite_250x250_png;  // first by convert assets to Binary Source in Projucer
    dataSize = BinaryData::sevent_blackwhite_250x250_pngSize;

    if (imageData != nullptr && dataSize > 0)
    {
        logo = juce::ImageFileFormat::loadFrom(imageData, dataSize);
    }
}

void SevenTAudioProcessorEditor::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment)
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
