#include "track.h"

Track::Track() {
	this->name = "";
	this->format = "";
	this->length = 0;
	this->filepath = URL();
}

Track::Track(String name, String format, int length, URL filepath) {
	this->name = name;
	this->format = format;
	this->length = length;
	this->filepath = filepath;
}

bool Track::operator==(const Track& other) const {
	return (name == other.name) &&
		(format == other.format) &&
		(length == other.length) &&
		(filepath == other.filepath);
}

String Track::getName() {
	return name;
}

void Track::setName(String name) {
	this->name = name;
}

String Track::getFormat() {
	return format;
}

void Track::setFormat(String format) {
	this->format = format;
}

int Track::getLength() {
	return length;
}

void Track::setLength(int length) {
	this->length = length;
}

URL Track::getURL() {
	return filepath;
}

void Track::setURL(URL newURL) {
	this->filepath = newURL;
}
