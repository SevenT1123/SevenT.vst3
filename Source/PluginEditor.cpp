/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SevenTAudioProcessorEditor::SevenTAudioProcessorEditor (SevenTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc(audioProcessor.apvts, "OSC1WAVETYPE", "OSC1VOLUME", "OSC1PHASEOFFSET", "OSC1DETUNE", "OSC1STEREO", "OSC1PAN")
    , adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , filter(audioProcessor.apvts, "FILTERTYPE", "FILTERFREQ", "FILTERRES")
    , unison(audioProcessor.apvts, "UNISONVOICES", "UNISONDETUNE", "UNISONBLEND", "UNISONSTEREO")
{
    setSize(1024, 768);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
	addAndMakeVisible(unison);

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
    const auto paddingY = 35;
    const auto paddingY2 = 370;

    osc.setBounds(paddingX, paddingY, 300, 300);
    adsr.setBounds(osc.getRight(), paddingY2, 250, 200);
    filter.setBounds(paddingX, paddingY2, 300, 200);
    unison.setBounds(osc.getRight(), paddingY, 305, 175);
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