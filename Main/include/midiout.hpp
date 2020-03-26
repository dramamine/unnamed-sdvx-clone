#pragma once
#include "Input.hpp"


/*
	The object responsible for drawing the track.
*/
class MidiOut {
public:
	// static MidiOut *instance;
	// RtMidiOut* port;

	static MidiOut* getInstance();

	// static RtMidiOut* openPort(std::string name);


	MidiOut() {
		// port = MidiOut::openPort("cpp-resolume");

	}
	~MidiOut() {
	}


	void updateBPM(float bpm);
  void sendUdp(std::string str);
	void sendMessage();
	void sendMessage(int a, int b, int c);
	void MidiOut::quickPress(uint32 b);
	void onButtonPressed(Input::Button b);
	void onButtonReleased(Input::Button b);
	void tick(float deltaTime);
};
