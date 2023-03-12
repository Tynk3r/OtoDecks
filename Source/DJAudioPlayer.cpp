/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 12 Mar 2023 12:38:34pm
    Author:  googa

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
    
}
DJAudioPlayer::~DJAudioPlayer()
{
}
void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    DBG(audioURL.toString(true));
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::play()
{
    transportSource.setPosition(0);
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double pos)
{
    double posInSecs = pos * transportSource.getLengthInSeconds();
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double gain)
{
    transportSource.setGain(gain);
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        // WRONG
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
