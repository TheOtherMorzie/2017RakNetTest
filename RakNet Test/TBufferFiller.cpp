#include "TBufferFiller.h"

#include "SoundPlayer.h"



TBufferFiller::TBufferFiller()
{
}


TBufferFiller::~TBufferFiller()
{
}

void TBufferFiller::setFillBuffer(std::mutex * bufferMutex, SP::WavBuffer * buffer)
{
	m_setBufferMutex.lock();
	m_bufferMutex = bufferMutex;
	m_buffer = buffer;
	m_setBufferMutex.unlock();
}
