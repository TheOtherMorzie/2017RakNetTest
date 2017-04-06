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

	void setFillBuffer(std::mutex * bufferMutex, SP::WavBuffer * buffer);

	virtual int getBufferSize() = 0;
	virtual int getTotalSize() = 0;
	virtual __int16 getSampleSize() = 0;
	virtual __int16 getChannelCount() = 0;

protected:

	std::mutex m_setBufferMutex;

	// buffer stuff
	std::mutex * m_bufferMutex = nullptr;
	SP::WavBuffer * m_buffer = nullptr;

};

