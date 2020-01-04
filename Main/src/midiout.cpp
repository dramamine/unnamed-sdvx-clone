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
MidiOut* instance;

MidiOut* MidiOut::getInstance() {
	if (!instance) {
		instance = new MidiOut();
	}
	return instance;
}

RtMidiOut* MidiOut::openPort(std::string name = "cpp-resolume")
{
	RtMidiOut* midiout = new RtMidiOut();
	// Check outputs.
	int nPorts = midiout->getPortCount();

	std::string portName;
	int myPort = -1;
	for (unsigned int i = 0; i < nPorts; i++)
	{
		try
		{
			portName = midiout->getPortName(i);
			if (portName.rfind(name) == 0)
			{
				myPort = i;
			}
		}
		catch (RtMidiError & error)
		{
			error.printMessage();
		}
	}

	if (myPort == -1)
		return nullptr;

	midiout->openPort(myPort);
	std::vector<unsigned char> message = { 144, 1, 1 };
	midiout->sendMessage(&message);
	Logf("opened port %d", Logger::Warning, myPort);
	return midiout;
}

void MidiOut::updateBPM(float bpm)
{
	
	uint8 bpmToSend = std::min(std::max(((uint8)bpm - 60) / 2.0, 0.), 127.);
	Logf("my bpm: %f %d", Logger::Warning, bpm, bpmToSend);
	std::vector<uint8> message = { 176, 119, bpmToSend };
	port->sendMessage(&message);
	int i = 0;
}

void MidiOut::sendMessage()
{
	std::vector<unsigned char> message = { 144, 1, 1 };
	port->sendMessage(&message);
	int i = 0;
}
