/*
  ==============================================================================

    WaveformDisplay.h
    Created: 12 Mar 2023 8:56:46pm
    Author:  googa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  :    public juce::Component, 
                            public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);

    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadURL(juce::URL audioURL);
    /** set the relative position of the play head*/
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumbnail;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
