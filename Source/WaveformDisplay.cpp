/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 12 Mar 2023 8:56:46pm
    Author:  googa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,juce::AudioThumbnailCache& cacheToUse): audioThumbnail(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    if (fileLoaded)
    {
        g.setColour(juce::Colours::lightgreen);
        audioThumbnail.drawChannel(g, getLocalBounds(), 0, audioThumbnail.getTotalLength(), 0, 1);
    }
    else
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds(), 1); // draw an outline around the component
        g.setColour(juce::Colours::green);
        g.setFont(24.0f);
        g.drawText("Waveform here!", getLocalBounds(), juce::Justification::centred, true); // draw some placeholder text
    }

}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}