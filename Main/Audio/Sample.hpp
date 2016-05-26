#pragma once
#include "AudioBase.hpp"

/*
	Audio sample, only supports wav files in native output frequency for now.
*/
class SampleRes : public AudioBase
{
public:
	static Ref<SampleRes> Create(class Audio* audio, const String& path);
	virtual ~SampleRes() = default;

public:
	// Plays this sample from the start
	virtual void Play() = 0;
	// Sets the playback volume
	virtual void SetVolume(float volume) = 0;
};

typedef Ref<SampleRes> Sample;