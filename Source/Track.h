#ifndef TRACK_H
#define TRACK_H

#pragma once
#include <string>
#include <JuceHeader.h>

class Track {
public:
	Track(String name, int length, URL filepath);

	String getName();
	void setName(String name);

	int getLength();
	void setLength(int length);

	URL getURL();
	void setURL(URL newURL);

private:
	String name;
	int length;
	URL filepath;
};

#endif
