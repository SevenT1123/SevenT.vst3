/*
  ==============================================================================

    PresetLoad.cpp
    Created: 4 Jan 2026 9:35:11am
    Author:  Seven T

  ==============================================================================
*/

#include "PresetLoad.h"

PresetLoad::PresetLoad(juce::AudioProcessorValueTreeState& apvts) : apvts(apvts), currentPreset("") {
    pathCheck();
}

void PresetLoad::savePreset(const juce::String& name) {
    if (name.isEmpty())
        return;

    auto presetFolder = getDefaultPresetFolder();
    auto presetFile = presetFolder.getChildFile(name + ".xml");

    auto state = apvts.copyState();

    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    if (xml != nullptr) {
        if (xml->writeTo(presetFile)) {
            currentPreset = name;
        }
    }
}

void PresetLoad::deletePreset(const juce::String& name) {
    if (name.isEmpty())
        return;

    auto presetFolder = getDefaultPresetFolder();
    auto presetFile = presetFolder.getChildFile(name + ".xml");

    if (presetFile.exists()) {
        if (presetFile.deleteFile()) {
            if (currentPreset == name)
                currentPreset = "";
        }
    }
}

void PresetLoad::loadPreset(const juce::String& name) {
    if (name.isEmpty())
        return;

    auto presetFolder = getDefaultPresetFolder();
    auto presetFile = presetFolder.getChildFile(name + ".xml");

    std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(presetFile));

    if (xml != nullptr) {
        auto tree = juce::ValueTree::fromXml(*xml);

        if (tree.isValid()) {
            apvts.replaceState(tree);
            currentPreset = name;
        }
    }
}

juce::StringArray PresetLoad::getAllPresets() const {
    juce::StringArray presets;
    auto presetFolder = getDefaultPresetFolder();

    if (presetFolder.exists()) {
        auto files = presetFolder.findChildFiles(juce::File::findFiles, false, "*.xml");

        for (auto& file : files) {
            presets.add(file.getFileNameWithoutExtension());
        }

        presets.sort(true);
    }

    return presets;
}

juce::File PresetLoad::getPresetFolder() const {
    return getDefaultPresetFolder();
}

juce::File PresetLoad::getDefaultPresetFolder() const {
    auto documentPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    return documentPath.getChildFile("SevenT").getChildFile("Presets");
}

void PresetLoad::pathCheck() {
    auto presetFolder = getDefaultPresetFolder();
}
