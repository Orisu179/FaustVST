
#include "EditorComponent.h"
EditorComponent::EditorComponent() : codeEditor(sourceCode, nullptr),
                                     workDir(juce::File::getSpecialLocation(juce::File::userHomeDirectory))
{
    addAndMakeVisible(&codeEditor);
    compileButton.setComponentID("compile");
    compileButton.setButtonText("compile");
    compileButton.onClick = [this] {
        if(onCompile) {
            onCompile();
        }
    };

    addAndMakeVisible(compileButton);
    importButton.setButtonText("Import");
    importButton.onClick = [this] {
        fileChooser = std::make_unique<juce::FileChooser>("Please select the DSP file you want to load..", workDir, "*.dsp");

        auto chooserFlag = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;
        fileChooser->launchAsync(chooserFlag, [this](const juce::FileChooser& chooser){
            auto sourceFile = chooser.getResult();
            setSource(sourceFile.loadFileAsString());
            workDir = sourceFile.getParentDirectory();
        });
    };
    addAndMakeVisible(importButton);
    exportButton.setButtonText("export");
    exportButton.onClick = [this]{
      fileChooser = std::make_unique<juce::FileChooser>("Chose where to save DSP file", workDir, "*.dsp");
      auto chooserFlag = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
      fileChooser->launchAsync(chooserFlag, [this](const juce::FileChooser& chooser) {
          auto sourceFile = chooser.getResult();
          auto outputStream = juce::FileOutputStream(sourceFile);
          sourceCode.writeToStream(outputStream);
          workDir = sourceFile.getParentDirectory();
      });
    };
    addAndMakeVisible(exportButton);
    revertButton.setButtonText("Revert");
    addAndMakeVisible(revertButton);

}

void EditorComponent::resized() {
    unsigned int margin = 10;
    unsigned int buttonHeight = 30;
    unsigned int buttonWidth = 100;

    using FB = juce::FlexBox;
    FB buttons;
    buttons.flexDirection = FB::Direction::row;
    buttons.alignItems = FB::AlignItems::flexStart;
    buttons.flexWrap = FB::Wrap::noWrap;
    buttons.justifyContent = FB::JustifyContent::flexStart;
    buttons.alignItems = FB::AlignItems::flexStart;
}

juce::String EditorComponent::getSource() {
    return sourceCode.getAllContent();
}

void EditorComponent::setSource (juce::String source) {
    sourceCode.replaceAllContent(source);
}