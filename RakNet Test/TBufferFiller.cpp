#include "TBufferFiller.h"

#include "SoundPlayer.h"



TBufferFiller::TBufferFiller()
{
}


TBufferFiller::~TBufferFiller()
{
}

void TBufferFiller::setFillBuffer(std::mutex * bufferMutex, SP::WavBuffer ** buffer)
{
	m_setBufferMutex.lock();
	m_bufferMutex = bufferMutex;
	m_buffer = buffer;
	m_setBufferMutex.unlock();
}

SP::WavBuffer ** TBufferFiller::getBuffer()
{
	return m_buffer;
}

std::mutex * TBufferFiller::getBufferMutex()
{
	return m_bufferMutex;
}
