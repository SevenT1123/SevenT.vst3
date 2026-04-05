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
    setSize(1024, 768);
    addAndMakeVisible(presetComponent);
    addAndMakeVisible(osc);
    addAndMakeVisible(osc2);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);

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

    presetComponent.setBounds(presetPanelX, presetPanelY, presetPanelWidth, presetPanelHeight);

    osc.setBounds(paddingX, paddingY, 405, 300);
    osc2.setBounds(osc.getRight(), paddingY, 405, 300);
    adsr.setBounds(osc.getRight(), paddingY2, 250, 200);
    filter.setBounds(paddingX, paddingY2, 300, 200);
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