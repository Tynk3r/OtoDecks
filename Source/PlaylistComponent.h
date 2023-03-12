#pragma once

#include <JuceHeader.h>
#include "Track.h"

class PlaylistComponent  :  public Component, 
                            public TableListBoxModel, 
                            public Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    void cellClicked(int rowNumber, int columnId, const MouseEvent&);
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
    void buttonClicked(Button* button) override;

    void FilterTracks();

private:
    TableListBox tableComponent;
    std::vector<Track> tracks;
    std::vector<Track> filteredTracks;
    TextButton loadButton{ "ADD TRACK" };
    TextButton clearButton{ "CLEAR TRACKS" };
    Label searchLabel;
    Label searchBar;
    String searchText = "";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
