// midiout.cpp
#include <iostream>
#include <cstdlib>
#include <map>

#include "stdafx.h"

#include "midiout.hpp"
#include "portmidi.h"
#include "mmeapi.h"
#include "Windows.h"
#include "E:\git\rtmidi-4.0.0\RtMidi.cpp"

RtMidiOut* port;

void MidiOut::openPort(std::string name)
{	
	RtMidiOut* midiout = new RtMidiOut();
	// Check outputs.
	int nPorts = midiout->getPortCount();

	std::string portName;
	int myPort = -1;
	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = midiout->getPortName(i);
			if (portName.rfind(name) >= 0) {
				myPort = i;
			}
		}
		catch (RtMidiError & error) {
			error.printMessage();
		}
	}

	if (myPort == -1) return;

	midiout->openPort(myPort);
	port = midiout;
}
void MidiOut::sendMessage()
{
	std::vector<unsigned char> message = { 144, 1, 1 };
	port->sendMessage(&message);
	int i = 0;

}

MidiOut::MidiOut()
{
	MidiOut::openPort("cpp-resolume");
}

MidiOut::~MidiOut()
{
  //delete midiout;
}
