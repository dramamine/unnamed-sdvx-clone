#include "stdafx.h"
#include "AudioStreamBase.hpp"

#define DR_WAV_IMPLEMENTATION
#include "extras/dr_wav.h"   // Enables WAV decoding.
#define DR_FLAC_IMPLEMENTATION
#include "extras/dr_flac.h"  // Enables FLAC decoding.
#define DR_MP3_IMPLEMENTATION
#include "extras/dr_mp3.h"   // Enables MP3 decoding.

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

class AudioStreamMA_Impl : public AudioStreamBase
{
private:
	Buffer m_Internaldata;
	float* m_pcm;

	int64 m_playbackPointer = 0;
	uint64 m_dataPosition = 0;

	const int sample_rate = 48000;
	ma_decoder m_decoder;
	int m_byteRate;

public:
	~AudioStreamMA_Impl()
	{
		Deregister();
	}

	bool Init(Audio* audio, const String& path, bool preload)
	{
		if (!AudioStreamBase::Init(audio, path, preload)) // Always preload for now
			return false;

		ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 2, sample_rate);
		ma_result result;

		if (m_preloaded)
		{
			result = ma_decode_memory((void*)m_data.data(), m_file.GetSize(), &config, &m_samplesTotal, (void**)&m_pcm);
		}
		else
		{			
			result = ma_decoder_init_file(*path, &config, &m_decoder);
		}

		if (result != MA_SUCCESS)
		{
			return false;
		}

		InitSampling(sample_rate);
		return true;
	}
	virtual int32 GetStreamPosition_Internal()
	{
		return m_playbackPointer;
	}
	virtual int32 GetStreamRate_Internal()
	{
		return sample_rate;
	}
	virtual void SetPosition_Internal(int32 pos)
	{
		if (pos < 0)
			m_playbackPointer = 0;
		else
			m_playbackPointer = pos;


		if (!m_preloaded)
		{
			//seek_to_pcm_frame is very slow
			//we want to use ma_decoder_seek_bytes_64
			//but we require some stuff to do that properly

			//ma_decoder_seek_to_pcm_frame(&m_decoder, pos);
			return;
		}
	}

	virtual int32 DecodeData_Internal()
	{
		if (m_preloaded)
		{
			uint32 samplesPerRead = 128;
			int actualRead = 0;
			for (size_t i = 0; i < samplesPerRead; i++)
			{
				if (m_playbackPointer >= m_samplesTotal)
				{
					m_currentBufferSize = samplesPerRead;
					m_remainingBufferData = samplesPerRead;
					return i;
				}
				m_readBuffer[0][i] = m_pcm[m_playbackPointer * 2];
				m_readBuffer[1][i] = m_pcm[m_playbackPointer * 2 + 1];
				m_playbackPointer++;
				actualRead++;
			}
			m_currentBufferSize = samplesPerRead;
			m_remainingBufferData = samplesPerRead;
			return samplesPerRead;
		}
		else
		{
			uint32 samplesPerRead = 128;
			float decodeBuffer[256];

			int totalRead = ma_decoder_read_pcm_frames(&m_decoder, decodeBuffer, samplesPerRead);
			for (size_t i = 0; i < totalRead; i++)
			{
				m_readBuffer[0][i] = decodeBuffer[i * 2];
				m_readBuffer[1][i] = decodeBuffer[i * 2 + 1];
			}
			m_currentBufferSize = totalRead;
			m_remainingBufferData = totalRead;
			return totalRead;
		}
		return 0;
	}
	float* GetPCM_Internal()
	{
		return m_pcm;
	}
	uint32 GetSampleRate_Internal() const
	{
		return sample_rate;
	}
};

class AudioStreamRes* CreateAudioStream_ma(class Audio* audio, const String& path, bool preload)
{
	AudioStreamMA_Impl* impl = new AudioStreamMA_Impl();
	if (!impl->Init(audio, path, preload))
	{
		delete impl;
		impl = nullptr;
	}
	return impl;
}