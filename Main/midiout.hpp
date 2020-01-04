#pragma once

/*
	The object responsible for drawing the track.
*/
class MidiOut : public Unique
{
public:
  MidiOut();
  ~MidiOut();
  void openPort(std::string);
  void sendMessage();
};
