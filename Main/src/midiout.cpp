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
#include "Input.hpp"

RtMidiOut* port;
MidiOut* instance;

// increase to speed out faster
// 60 would take 127/60 seconds
float SPEED_OF_EASE_OUT = 150.;

int CC_CONTROL_CH1 = 144;

std::map<Input::Button, int> buttonMidiControl;
std::map<Input::Button, int> buttonValues;
std::map<Input::Button, bool> buttonPressed;

MidiOut* MidiOut::getInstance() {
	if (!instance) {
		instance = new MidiOut();
		// should match with the dashboard 
		buttonMidiControl[Input::Button::BT_0] = 41;
		buttonMidiControl[Input::Button::FX_0] = 42;
		buttonMidiControl[Input::Button::BT_1] = 43;
		buttonMidiControl[Input::Button::BT_2] = 44;
		buttonMidiControl[Input::Button::FX_1] = 45;
		buttonMidiControl[Input::Button::BT_3] = 46;

		buttonValues[Input::Button::BT_0] = 0;
		buttonValues[Input::Button::FX_0] = 0;
		buttonValues[Input::Button::BT_1] = 0;
		buttonValues[Input::Button::BT_2] = 0;
		buttonValues[Input::Button::FX_1] = 0;
		buttonValues[Input::Button::BT_3] = 0;

		buttonPressed[Input::Button::BT_0] = false;
		buttonPressed[Input::Button::FX_0] = false;
		buttonPressed[Input::Button::BT_1] = false;
		buttonPressed[Input::Button::BT_2] = false;
		buttonPressed[Input::Button::FX_1] = false;
		buttonPressed[Input::Button::BT_3] = false;
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

void MidiOut::sendMessage(int a, int b, int c)
{
	Logf("from midi: %d,%d,%d", Logger::Warning, a, b, c);
	std::vector<unsigned char> message = {(unsigned char)a, (unsigned char)b, (unsigned char)c};
	port->sendMessage(&message);
}

void MidiOut::onButtonPressed(Input::Button b)
{
	buttonValues[b] = 127;
	buttonPressed[b] = true;
	sendMessage(CC_CONTROL_CH1, buttonMidiControl[b], buttonValues[b]);
}

void MidiOut::onButtonReleased(Input::Button b)
{
	// buttonValues[b] = 0;
	buttonPressed[b] = false;
	sendMessage(CC_CONTROL_CH1, buttonMidiControl[b], buttonValues[b]);
}

void MidiOut::tick(float deltaTime)
{
	Logf("dt: %f", Logger::Warning, deltaTime);
	for (const auto &myPair : buttonPressed)
	{
		Input::Button b = myPair.first;
		if (!myPair.second && buttonValues[b] > 0) {
			buttonValues[b] = std::max(0, (int)(buttonValues[b] - SPEED_OF_EASE_OUT*deltaTime));
			sendMessage(CC_CONTROL_CH1, buttonMidiControl[b], buttonValues[b]);
		}
	}
}
