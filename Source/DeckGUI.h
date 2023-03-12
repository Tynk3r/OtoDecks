#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, 
           AudioFormatManager & 	formatManagerToUse,
           AudioThumbnailCache & 	cacheToUse );
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked (Button *) override;
    void loadURL(URL audioURL);
    void sliderValueChanged (Slider *slider) override;
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 
    void timerCallback() override; 

private:
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
  
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;

    TextButton loopButton{ "LOAD A TRACK!" };

    FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player; 

    URL currentTrack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
