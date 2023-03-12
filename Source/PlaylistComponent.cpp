/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 12 Mar 2023 10:41:19pm
    Author:  googa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("Filename", 1, 400);
    tableComponent.getHeader().addColumn("Length", 2, 150);
    tableComponent.getHeader().addColumn("", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 150);

    tableComponent.setModel(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    addAndMakeVisible(clearButton);
    clearButton.addListener(this);

    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");
    trackTitles.push_back("Track 4");
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    float height = getHeight() / 8;
    tableComponent.setBounds(0, 0, getWidth(), height * 7);
    loadButton.setBounds(0, height * 7, getWidth() / 2, height);
    clearButton.setBounds(getWidth() / 2, height * 7, getWidth() / 2, height);
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    switch (columnId)
    {
    case 1: // Filename
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    case 2: // Length
        g.drawText("long", 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    default:
        break;
    }
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{

}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton("Play on 1");
            existingComponentToUpdate = btn;
            btn->addListener(this);
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadButton)
    {
        trackTitles.push_back("Track 5");
        tableComponent.updateContent();
        tableComponent.repaint();
    }
}
