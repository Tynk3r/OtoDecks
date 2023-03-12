#pragma once

#include <fstream>
#include <vector>
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
    void selectedRowsChanged(int row);
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
    void buttonClicked(Button* button) override;

    void FilterTracks();

    Track loadURL(URL audioURL);

    void saveTracksToCSV(std::vector<Track> tracks, std::string filename);

    std::vector<Track> loadTracksFromCSV(std::string filename);

private:
    TableListBox tableComponent;
    std::vector<Track> tracks;
    std::vector<Track> filteredTracks;
    TextButton deck1Button{ "PLAY SELECTED ON DECK 1" };
    TextButton deck2Button{ "PLAY SELECTED ON DECK 2" };
    TextButton loadButton{ "ADD TRACKS" };
    TextButton clearButton{ "CLEAR TRACKS" };
    Label searchLabel;
    Label searchBar;
    String searchText = "";
    FileChooser fChooser{ "Select a file..." };
    Track selectedTrack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
