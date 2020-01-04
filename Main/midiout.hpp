#pragma once
#include "E:\git\rtmidi-4.0.0\RtMidi.h"
/*
	The object responsible for drawing the track.
*/
class MidiOut {
public:
	// static MidiOut *instance;
	RtMidiOut* port;

	static MidiOut* getInstance();

	static RtMidiOut* openPort(std::string name);


	MidiOut() {
		port = MidiOut::openPort("cpp-resolume");
	}
	~MidiOut() {
	}


	void updateBPM(float bpm);

	void sendMessage();
};
