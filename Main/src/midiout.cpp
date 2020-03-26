// midiout.cpp
#include <iostream>
#include <cstdlib>
#include <map>

#include "stdafx.h"

#include "midiout.hpp"
#include "Input.hpp"

#include <boost/asio.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::address;

boost::asio::io_service io_service;
udp::socket s(io_service, udp::endpoint(udp::v4(), 0));
udp::endpoint remote_endpoint;
boost::system::error_code err;

MidiOut* instance;

// increase to speed out faster
// 60 would take 127/60 seconds
float SPEED_OF_EASE_OUT = 150.;

int CC_CONTROL_CH1 = 144;

std::map<Input::Button, int> buttonMidiControl;
std::map<Input::Button, std::string> buttonName;
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

	  buttonName[Input::Button::BT_0] = "button1";
		buttonName[Input::Button::FX_0] = "button2";
		buttonName[Input::Button::BT_1] = "button3";
		buttonName[Input::Button::BT_2] = "button4";
		buttonName[Input::Button::FX_1] = "button5";
		buttonName[Input::Button::BT_3] = "button6";

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

		remote_endpoint = udp::endpoint(address::from_string("127.0.0.1"), 7001);

		// socket.open(udp::v4());
		//sendUdp();
	}



	return instance;
}



void MidiOut::updateBPM(float bpm)
{
	sendUdp("bpm|" + std::to_string(bpm));
}

void MidiOut::sendUdp(std::string str) {
	auto sent = s.send_to(boost::asio::buffer(str), remote_endpoint, 0, err);
}

void MidiOut::sendMessage()
{

	std::vector<unsigned char> message = { 144, 1, 1 };
	//port->sendMessage(&message);
	int i = 0;
}

void MidiOut::quickPress(uint32 b)
{
	sendUdp("quickpress|" + std::to_string(b));
	sendUdp("quickunpress|" + std::to_string(b));
}

void MidiOut::sendMessage(int a, int b, int c)
{
	Logf("from midi: %d,%d,%d", Logger::Warning, a, b, c);
	std::vector<unsigned char> message = {(unsigned char)a, (unsigned char)b, (unsigned char)c};
	//port->sendMessage(&message);
}

void MidiOut::onButtonPressed(Input::Button b)
{
	buttonValues[b] = 127;
	buttonPressed[b] = true;
	//sendMessage(CC_CONTROL_CH1, buttonMidiControl[b], buttonValues[b]);
	sendUdp(buttonName[b] + "|1");
}

void MidiOut::onButtonReleased(Input::Button b)
{
	// buttonValues[b] = 0;
	buttonPressed[b] = false;
	//sendMessage(CC_CONTROL_CH1, buttonMidiControl[b], buttonValues[b]);
	sendUdp(buttonName[b] + "|0");
}

void MidiOut::tick(float deltaTime)
{
	Logf("dt: %f", Logger::Warning, deltaTime);
	for (const auto &myPair : buttonPressed)
	{
		Input::Button b = myPair.first;
		if (!myPair.second && buttonValues[b] > 0) {
			buttonValues[b] = std::max(0, (int)(buttonValues[b] - SPEED_OF_EASE_OUT*deltaTime));
			//sendMessage(CC_CONTROL_CH1, buttonMidiControl[b], buttonValues[b]);
		}
	}
}
