#pragma once

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#include <mutex>

#include "CommonTypes.h"

class TBufferFiller;

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	int init(int bufferSizeByte);

	/*
	This function is used to load data into fmod's internal buffer
	Fmod will then play that interal buffer
	Once the internal buffer is played out this function will be called again
	*/
	FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND* sound, void *data, unsigned int datalen);
	FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND* sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype);

	// will return true if buffers switch properly
	bool swapBuffer();

	// pauses the soundChannel
	void setPause(bool state);
	bool getPause();
	void togglePause();

protected:


	int m_numberOfChannels = 0;
	int m_sampleSize; // size of each sample
	int m_sampleIndex; // point along file (0:05 in x 44,100 samples per second means m_sampleIndex = 220,500)

	int m_bufferSizeByte = 0; // this needs to be a mutiple of channels multiplied by the sample size (small buffers can be problematic)
	unsigned char m_bufferState = 0x00;

	std::mutex m_bufferMutex;
	SP::WavBuffer * m_currentBuffer = nullptr;
	SP::WavBuffer * m_otherBuffer = nullptr;

	std::mutex m_soundMutex; // so that the callback functions don't do screwy things
	FMOD::Sound * m_sound = nullptr;
	std::mutex m_channelMutex; // so that the callback functions don't do screwy things
	FMOD::Channel * m_channel = nullptr;
	std::mutex m_systemMutex; // so that the callback functions don't do screwy things
	FMOD::System * m_system = nullptr;

	TBufferFiller * m_bufferFiller = nullptr;


};

