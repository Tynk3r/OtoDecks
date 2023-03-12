#ifndef TRACK_H
#define TRACK_H

#pragma once
#include <string>
#include <JuceHeader.h>

class Track {
public:
	Track();
	Track(String name, String format, int length, URL filepath);

	bool operator==(const Track& other) const;

	String getName();
	void setName(String name);

	String getFormat();
	void setFormat(String format);

	int getLength();
	void setLength(int length);

	URL getURL();
	void setURL(URL newURL);

private:
	String name;
	String format;
	int length;
	URL filepath;
};

#endif
