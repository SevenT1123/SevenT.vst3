/*
  ==============================================================================

    PresetComponent.cpp
    Created: 4 Jan 2026 9:35:46am
    Author:  Seven T

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PresetComponent.h"

//==============================================================================
PresetComponent::PresetComponent(PresetLoad& pl) : presetLoad(pl)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
     // Preset label
    presetLabel.setText("Preset:", juce::dontSendNotification);
    presetLabel.setFont(juce::Font(16.0f));
    presetLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    presetLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(presetLabel);

    // Preset dropdown
    presetList.setTextWhenNoChoicesAvailable("No Presets Available");
    presetList.setTextWhenNothingSelected("Select Preset...");
    addAndMakeVisible(presetList);
    presetList.addListener(this);

    // Save button
    saveButton.setButtonText("Save");
    saveButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgreen);
    saveButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(saveButton);
    saveButton.addListener(this);

    // Delete button
    deleteButton.setButtonText("Delete");
    deleteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkred);
    deleteButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    addAndMakeVisible(deleteButton);
    deleteButton.addListener(this);

    // Initial update
    updatePresetList();
}

PresetComponent::~PresetComponent()
{
    presetList.removeListener(this);
    saveButton.removeListener(this);
    deleteButton.removeListener(this);
}

void PresetComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    // Draw background
    g.fillAll(juce::Colours::black);

    // Draw border
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

void PresetComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto bounds = getLocalBounds().reduced(5);

    const int labelWidth = 60;
    const int buttonWidth = 70;
    const int spacing = 5;

    // Layout: [Label] [Dropdown] [Save] [Delete]
    presetLabel.setBounds(bounds.removeFromLeft(labelWidth));
    bounds.removeFromLeft(spacing);

    deleteButton.setBounds(bounds.removeFromRight(buttonWidth));
    bounds.removeFromRight(spacing);

    saveButton.setBounds(bounds.removeFromRight(buttonWidth));
    bounds.removeFromRight(spacing);

    presetList.setBounds(bounds);
}

void PresetComponent::buttonClicked(juce::Button* button)
{
    if (button == &saveButton)
    {
        showSaveDialog();
    }
    else if (button == &deleteButton)
    {
        showDeleteConfirmation();
    }
}

void PresetComponent::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &presetList)
    {
        auto selectedPreset = presetList.getText();
        if (selectedPreset.isNotEmpty())
        {
            presetLoad.loadPreset(selectedPreset);
        }
    }
}

void PresetComponent::updatePresetList()
{
    presetList.clear(juce::dontSendNotification);

    auto presets = presetLoad.getAllPresets();

    for (int i = 0; i < presets.size(); ++i)
    {
        presetList.addItem(presets[i], i + 1);
    }

    // Select current preset if it exists
    auto currentPreset = presetLoad.getCurrentPreset();
    if (currentPreset.isNotEmpty())
    {
        presetList.setText(currentPreset, juce::dontSendNotification);
    }
}

void PresetComponent::showSaveDialog() {
    // Create alert window for entering preset name (non-modal)
    auto* window = new juce::AlertWindow("Save Preset",
        "Enter a name for your preset:",
        juce::AlertWindow::NoIcon);

    window->addTextEditor("presetName", presetLoad.getCurrentPreset(), "Preset Name:");
    window->addButton("Save", 1, juce::KeyPress(juce::KeyPress::returnKey));
    window->addButton("Cancel", 0, juce::KeyPress(juce::KeyPress::escapeKey));

    // Use async callback instead of modal loop
    window->enterModalState(true,
        juce::ModalCallbackFunction::create([this, window](int result)
            {
                if (result == 1) // Save button clicked
                {
                    auto presetName = window->getTextEditorContents("presetName").trim();

                    if (presetName.isNotEmpty())
                    {
                        // Check if preset already exists
                        auto existingPresets = presetLoad.getAllPresets();

                        if (existingPresets.contains(presetName))
                        {
                            // Ask for overwrite confirmation
                            juce::AlertWindow::showOkCancelBox(
                                juce::AlertWindow::WarningIcon,
                                "Overwrite Preset?",
                                "A preset named '" + presetName + "' already exists. Do you want to overwrite it?",
                                "Overwrite",
                                "Cancel",
                                nullptr,
                                juce::ModalCallbackFunction::create([this, presetName](int overwriteResult)
                                    {
                                        if (overwriteResult == 1) // Overwrite confirmed
                                        {
                                            presetLoad.savePreset(presetName);
                                            updatePresetList();
                                            presetList.setText(presetName, juce::dontSendNotification);
                                        }
                                    })
                            );
                        }
                        else
                        {
                            // No conflict, save directly
                            presetLoad.savePreset(presetName);
                            updatePresetList();
                            presetList.setText(presetName, juce::dontSendNotification);
                        }
                    }
                }

                // Delete window after use
                delete window;
            }),
        true // Delete window on dismiss
    );
}

void PresetComponent::showDeleteConfirmation()
{
    auto selectedPreset = presetList.getText();

    if (selectedPreset.isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon,
            "No Preset Selected",
            "Please select a preset to delete.",
            "OK"
        );
        return;
    }

    // Use async callback
    juce::AlertWindow::showOkCancelBox(
        juce::AlertWindow::WarningIcon,
        "Delete Preset?",
        "Are you sure you want to delete the preset '" + selectedPreset + "'?",
        "Delete",
        "Cancel",
        nullptr,
        juce::ModalCallbackFunction::create([this, selectedPreset](int result)
            {
                if (result == 1) // Delete confirmed
                {
                    presetLoad.deletePreset(selectedPreset);
                    updatePresetList();
                    presetList.setText("", juce::dontSendNotification);
                }
            })
    );
}
