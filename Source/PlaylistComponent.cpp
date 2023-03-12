#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    addAndMakeVisible(searchLabel);
    searchLabel.setText("SEARCH FOR ->", dontSendNotification);
    searchLabel.setJustificationType(Justification::centred);
    searchLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(searchBar);
    searchBar.setEditable(true);
    searchBar.setColour(juce::Label::backgroundColourId, juce::Colours::lightgrey);
    searchBar.onTextChange = [this] { searchText = searchBar.getText().toLowerCase(); FilterTracks(); };

    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("Filename", 1, 400);
    tableComponent.getHeader().addColumn("Length", 2, 100);
    tableComponent.getHeader().addColumn("", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 150);

    tableComponent.setModel(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    addAndMakeVisible(clearButton);
    clearButton.addListener(this);

    tracks.push_back(Track("Track 1", 2, URL()));
    FilterTracks();
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
    searchLabel.setBounds(0, 0, getWidth() * 0.25f, height);
    searchBar.setBounds(getWidth() * 0.25f, 0, getWidth() * 0.75f, height);
    tableComponent.setBounds(0, height, getWidth(), height * 6);
    loadButton.setBounds(0, height * 7, getWidth() / 2, height);
    clearButton.setBounds(getWidth() / 2, height * 7, getWidth() / 2, height);
}

int PlaylistComponent::getNumRows()
{
    return filteredTracks.size();
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
        g.drawText(filteredTracks[rowNumber].getName(), 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    case 2: // Length
        g.drawText(std::to_string(filteredTracks[rowNumber].getLength()), 2, 0, width - 4, height, Justification::centredLeft, true);
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
    if (existingComponentToUpdate == nullptr) {
        TextButton* btn = nullptr;
        if (columnId == 3) {
            btn = new TextButton("Play on 1");
        }
        else if (columnId == 4) {
            btn = new TextButton("Play on 2");
        }
        if (btn != nullptr) {
            btn->addListener(this);
            btn->setComponentID(String(rowNumber));
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &loadButton)
    {
        tracks.push_back(Track("Track 2", 2, URL()));
    }
    FilterTracks();
}

void PlaylistComponent::FilterTracks()
{
    if (!filteredTracks.empty())
    {
        filteredTracks.clear();
    }
    if (searchText.isEmpty())
    {
        filteredTracks = tracks;
    }
    else
    {
        if (!tracks.empty())
        {
            for (Track t : tracks)
            {
                if (t.getName().toLowerCase().contains(searchText))
                {
                    filteredTracks.push_back(t);
                }
            }
        }        
    }
    tableComponent.updateContent();
    tableComponent.repaint();
}
