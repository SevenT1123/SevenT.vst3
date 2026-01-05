/*
  ==============================================================================

    PresetComponent.h
    Created: 4 Jan 2026 9:35:46am
    Author:  Seven T

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Data/PresetLoad.h"

//==============================================================================
/*
*/
class PresetComponent : public juce::Component, public juce::Button::Listener, public juce::ComboBox::Listener
{
public:
    PresetComponent(PresetLoad& pl);
    ~PresetComponent() override;

    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBox) override;

    void updatePresetList();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PresetLoad& presetLoad;

    juce::ComboBox presetList;
    juce::TextButton saveButton;
    juce::TextButton deleteButton;
    juce::Label presetLabel;

    void showSaveDialog();
    void showDeleteConfirmation();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetComponent)
};
