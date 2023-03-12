#include "track.h"

Track::Track(String name, int length, URL filepath) {
	this->name = name;
	this->length = length;
	this->filepath = filepath;
}

String Track::getName() {
	return name;
}

void Track::setName(String name) {
	this->name = name;
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
