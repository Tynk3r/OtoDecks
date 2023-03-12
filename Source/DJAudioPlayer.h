/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 12 Mar 2023 12:38:34pm
    Author:  googa

  ==============================================================================
*/

#pragma once
// assumes your JuceHeader file is here
// check your MainComponent.h to see
// how the include is configured on your system
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer :   public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    void loadURL(juce::URL file);           // Load file path
    void play();
    void stop();
    void setPosition(double pos);     
    void setGain(double gain);        
    void setSpeed(double ratio);      

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    /** get the relative position of the play head */
    double getPositionRelative();

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource; 
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};