/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse
) : player(_player),
waveformDisplay(formatManagerToUse, cacheToUse)
{
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);

    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5); 
    volSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 100, 100);

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1, 10);
    speedSlider.setValue(1);
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 100, 100);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posSlider.setValue(0);
    posSlider.setSliderStyle(Slider::LinearHorizontal);
    posSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 100, 100);

    addAndMakeVisible(loopButton);
    loopButton.addListener(this);

    addAndMakeVisible(waveformDisplay);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics& g)
{
    // Draw the background gradient
    ColourGradient gradient(Colours::lightcoral, getWidth() / 2, 0, Colours::forestgreen, getWidth() / 2, getHeight(), false);
    g.setGradientFill(gradient);
    g.fillRect(getLocalBounds());

    g.setColour(Colours::white);
    double rowH = getHeight() / 8;
    double colW = getWidth() / 2;
    g.drawText(currentTrack.getFileName(), colW, rowH * 2, colW, rowH, Justification::centred, true);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    double colW = getWidth() / 2;

    playButton.setBounds(0, 0, colW, rowH * 2);
    stopButton.setBounds(0, rowH * 2, colW, rowH * 2);

    volSlider.setBounds(colW, 0, colW, rowH);
    speedSlider.setBounds(colW, rowH, colW, rowH);
    //posSlider.setBounds(colW, rowH * 2, colW, rowH);

    loopButton.setBounds(colW, rowH * 3, colW, rowH);

    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 4);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }
    else if (button == &stopButton)
    {
        player->stop();
    }
    else if (button == &loopButton)
    {
        if (player->isPlaying())
        {
            player->toggleLoop();
            if (player->getLooping())
            {
                button->setButtonText("TURN OFF LOOP");
            }
            else
            {
                button->setButtonText("TURN ON LOOP");
            }
            button->repaint();
        }
    }
    /*else if (button == &loadButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                loadURL(URL{ chooser.getResult() });
            });
    }*/ // deprecated since we added playlist
}

void DeckGUI::loadURL(URL audioURL)
{
    currentTrack = audioURL;
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
    loopButton.setButtonText("TURN ON LOOP");
    loopButton.repaint();
    repaint();
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        player->loadURL(URL{ File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
        player->getPositionRelative());
}