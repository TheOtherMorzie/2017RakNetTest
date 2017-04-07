#pragma once
#include "ThreadBase.h"

#include <mutex>

#include "CommonTypes.h"

class SoundPlayer;

class TBufferFiller : public ThreadBase
{
public:

	TBufferFiller();
	virtual ~TBufferFiller();

	void setFillBuffer(std::mutex * bufferMutex, SP::WavBuffer ** buffer);

	virtual int getBufferSize() = 0; // size of a single buffer
	virtual int getTotalSize() = 0; // size of entire sound file (only PCM data) in bytes
	virtual int getSampleSize() = 0; // size of a single sample in bytes
	virtual int getChannelCount() = 0; // number of channels
	virtual int getPlayBackRate() = 0; // playback rate of file
	virtual int getFormat() = 0; // get data format

protected:

	SP::WavBuffer ** getBuffer();
	std::mutex * getBufferMutex();

	std::mutex m_setBufferMutex;

	// buffer stuff
	std::mutex * m_bufferMutex = nullptr;
	SP::WavBuffer ** m_buffer = nullptr; // this needs to be a point to a pointer so that when the buffer is switched, this is switched as well

};

