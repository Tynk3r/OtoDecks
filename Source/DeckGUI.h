/*
  ==============================================================================

    DeckGUI.h
    Created: 12 Mar 2023 1:10:42pm
    Author:  googa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget
{
public:
    DeckGUI(DJAudioPlayer* player,
    juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    /* Component Virtuals */
    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    DJAudioPlayer* djAudioPlayer;
    WaveformDisplay waveformDisplay;

    // Buttons
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    // Sliders
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;

    // Non-Direct Input Helpers
    juce::FileChooser fChooser{ "Select a file..." };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
