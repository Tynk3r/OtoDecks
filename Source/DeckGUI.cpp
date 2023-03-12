/*
  ==============================================================================

    DeckGUI.cpp
    Created: 12 Mar 2023 1:10:42pm
    Author:  googa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,juce::AudioFormatManager& formatManagerToUse,juce::AudioThumbnailCache& cacheToUse): djAudioPlayer(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // Init User Input
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    addAndMakeVisible(volumeSlider);
    volumeSlider.setValue(0.5);
    volumeSlider.setRange(0, 1);
    volumeSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    positionSlider.setValue(0);
    positionSlider.setRange(0, 1);
    positionSlider.addListener(this);
    addAndMakeVisible(speedSlider);
    speedSlider.setValue(1);
    speedSlider.setRange(0.1, 10);
    speedSlider.addListener(this);

    addAndMakeVisible(waveformDisplay);
}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volumeSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    positionSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        djAudioPlayer->setPosition(0);
        djAudioPlayer->play();
    }
    else if (button == &stopButton)
    {
        djAudioPlayer->stop();
    }
    else if (button == &loadButton)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                djAudioPlayer->loadURL(juce::URL{ chooser.getResult() });
                // and now the waveformDisplay as well
                waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
            });

    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        djAudioPlayer->setGain(slider->getValue());
    }
    else if (slider == &positionSlider)
    {
        djAudioPlayer->setPosition(slider->getValue());
    }
    else if (slider == &speedSlider)
    {
        djAudioPlayer->setSpeed(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (juce::String filename : files)
    {
        DBG(filename + "\n");
        juce::URL fileURL = juce::URL{ juce::File{filename} };
        djAudioPlayer->loadURL(fileURL);
        return;
    }
}

