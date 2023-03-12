#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // Search Bar
    addAndMakeVisible(searchLabel);
    searchLabel.setText("SEARCH FOR ->", dontSendNotification);
    searchLabel.setJustificationType(Justification::centred);
    searchLabel.setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
    addAndMakeVisible(searchBar);
    searchBar.setEditable(true);
    searchBar.setColour(juce::Label::backgroundColourId, juce::Colours::lightgrey);
    searchBar.onTextChange = [this] { searchText = searchBar.getText().toLowerCase(); FilterTracks(); };

    // Table and Columns
    addAndMakeVisible(tableComponent);
    auto columnFlags = TableHeaderComponent::notResizableOrSortable;
    tableComponent.getHeader().addColumn("#",           1, 30, 0, 0, columnFlags);
    tableComponent.getHeader().addColumn("Filename",    2, 200, 0, 0, columnFlags);
    tableComponent.getHeader().addColumn("Format",      3, 50, 0, 0, columnFlags);
    tableComponent.getHeader().addColumn("Length",      4, 100, 0, 0, columnFlags);
    tableComponent.setModel(this);

    // Load and Clear Buttons
    addAndMakeVisible(deck1Button);
    deck1Button.addListener(this);
    addAndMakeVisible(deck2Button);
    deck2Button.addListener(this);
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    addAndMakeVisible(clearButton);
    clearButton.addListener(this);

    // Track
    selectedTrack = Track();
    tracks = loadTracksFromCSV("trackurls.csv");

    FilterTracks();
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics& g)
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
    float width = getWidth() / 2;
    searchLabel.setBounds(0, 0, getWidth() * 0.125f, height);
    searchBar.setBounds(getWidth() * 0.125f, 0, getWidth() * 0.375f, height);

    tableComponent.setBounds(0, height, width, height * 7);

    deck1Button.setBounds(width, 0, getWidth() / 2, height * 2);
    deck2Button.setBounds(width, height * 2, getWidth() / 2, height * 2);
    loadButton.setBounds(width, height * 4, getWidth() / 2, height * 2);
    clearButton.setBounds(width, height * 6, getWidth() / 2, height * 2);
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
    g.drawRect(0, 0, width, height);
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    switch (columnId)
    {
    case 1: // #
        g.drawText(String(rowNumber), 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    case 2: // Filename
        g.drawText(filteredTracks[rowNumber].getName(), 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    case 3: // Format
        g.drawText(filteredTracks[rowNumber].getFormat(), 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    case 4: // Length
        g.drawText(std::to_string(filteredTracks[rowNumber].getLength()) + "s", 2, 0, width - 4, height, Justification::centredLeft, true);
        break;
    default:
        break;
    }
    g.drawRect(0, 0, width, height);
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const MouseEvent&)
{
}

void PlaylistComponent::selectedRowsChanged(int row)
{
    if (!filteredTracks.empty())
    {
        selectedTrack = filteredTracks[row];
    DBG("Selected track: " + selectedTrack.getName());
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &deck1Button)
    {
        
    }
    else if (button == &deck2Button)
    {
        
    }
    else if (button == &loadButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                if (!chooser.getResults().isEmpty())
                {
                    for (File f : chooser.getResults())
                    {
                        Track addedTrack = loadURL(URL{ f });
                        if (tracks.empty() || std::find(tracks.begin(), tracks.end(), addedTrack) == tracks.end())
                        {
                            tracks.push_back(addedTrack);
                        }
                    }
                    FilterTracks();                    
                }                
            });
    }
    else if (button == &clearButton)
    {
        if (!tracks.empty())
        {
            tracks.clear();
        }
        FilterTracks();
    }
}

void PlaylistComponent::FilterTracks()
{
    saveTracksToCSV(tracks, "trackurls.csv");
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

Track PlaylistComponent::loadURL(URL audioURL)
{
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats(); // register basic audio formats (e.g. WAV, AIFF)

    std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(audioURL.createInputStream(false))); // create an input stream from the URL and try to create a reader from the stream

    if (reader.get() != nullptr) // if reader was created successfully
    {
        const int lengthInSeconds = reader->lengthInSamples / reader->sampleRate; // calculate length in seconds
        const String fileFormat = reader->getFormatName(); // get file format name

        return Track(audioURL.getFileName(), fileFormat, lengthInSeconds, audioURL);
    }
    else
    {
        std::cout << "Failed to load audio file from URL: " << audioURL.toString(true) << std::endl;
    }
}

void PlaylistComponent::saveTracksToCSV(std::vector<Track> tracks, std::string filename) {
    std::ofstream outfile(filename, std::ofstream::trunc);
    if (outfile) {
        for (auto track : tracks) {
            outfile << track.getURL().toString(false) << std::endl;
        }
        outfile.close();
    }
}

std::vector<Track> PlaylistComponent::loadTracksFromCSV(std::string filename) {
    std::vector<Track> tracks;
    std::ifstream infile(filename);
    if (infile) {
        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;
            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() == 1) {
                URL filepath(tokens[0]);
                tracks.push_back(loadURL(filepath));
            }
        }
        infile.close();
    }
    return tracks;
}