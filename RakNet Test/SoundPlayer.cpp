#include "SoundPlayer.h"


#include <cstring> // for low level memory functions (ie. memcpy)


#include "ExceptionC.h"
#include "TBufferFiller.h"


SoundPlayer::SoundPlayer()
{
}


SoundPlayer::~SoundPlayer()
{
}

int SoundPlayer::init(int bufferSizeByte)
{
	m_bufferSizeByte = bufferSizeByte;

	m_currentBuffer = new SP::WavBuffer(m_bufferSizeByte);
	m_otherBuffer = new SP::WavBuffer(m_bufferSizeByte);


	return 0;
}

FMOD_RESULT F_CALLBACK SoundPlayer::pcmreadcallback(FMOD_SOUND * sound, void * data, unsigned int datalen)
{
	EVALUATEC(m_currentBuffer != nullptr);
	EVALUATEC(m_otherBuffer != nullptr);

	//FMOD::Sound *snd = (FMOD::Sound *)sound;

	if (m_currentBuffer->m_isDone) // buffers have not been switched yet
	{
		if (swapBuffer()) // buffers sucessfully switched, should continue
		{

		}
		else // buffer failed to swap (other buffer is not filled)
		{
			m_bufferState |= SP::BSBF_WAITING_ON_OTHER_BUFFER;
			// to stop it looping over the last loaded buffer
			setPause(true);
			while (swapBuffer() == false)
			{
				// give the program a chance to swap the buffers and to avoid locking up the cpu
				std::chrono::milliseconds timespan(100);
				std::this_thread::sleep_for(timespan);
			}
			setPause(false);
		}
	}

	// the current buffer should not be done
	EVALUATEC(m_currentBuffer->m_isDone);

	// load m_current buffer into data


	// this will loop though data allowing the audio buffer to be set
	signed short *stereo16bitbuffer = (signed short *)data;
	for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
	{
		//*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
		//*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel
		memcpy(&(*stereo16bitbuffer++), &(m_currentBuffer->m_data[count * 2]), m_sampleSize); // left channel
		memcpy(&(*stereo16bitbuffer++), &(m_currentBuffer->m_data[(count * 2) + 1]), m_sampleSize); // right channel

		m_sampleIndex++;
	}


	// old
	bool callBackDone = false;
	while (!callBackDone)
	{
		if (!m_currentBuffer->m_isDone) // play sample from current buffer
		{
			// play sample

			// this will loop though data allowing the audio buffer to be set
			signed short *stereo16bitbuffer = (signed short *)data;
			for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
			{
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel
				memcpy(&(*stereo16bitbuffer++), &(m_currentBuffer->m_data[count * 2]), m_sampleSize); // left channel
				memcpy(&(*stereo16bitbuffer++), &(m_currentBuffer->m_data[(count * 2) + 1]), m_sampleSize); // right channel
			}

			//
			m_sampleIndex++; // has played a sample and should move on
			callBackDone = true;
		}
		else // switch buffers
		{
			bool otherFilled = swapBuffer();
			if (!otherFilled) // still on same buffer
			{
				
				m_bufferState |= SP::BSBF_WAITING_ON_OTHER_BUFFER;
				callBackDone = true;
			}
			// won't exit while loop so that the new buffer can play it's sample
		}
	}


	// ############################# example code
	//static float  t1 = 0, t2 = 0;        // time
	//static float  v1 = 0, v2 = 0;        // velocity
	//signed short *stereo16bitbuffer = (signed short *)data;

	//for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
	//{
	//	*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
	//	*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel

	//	t1 += 0.01f + v1;
	//	t2 += 0.0142f + v2;
	//	v1 += (float)(Common_Sin(t1) * 0.002f);
	//	v2 += (float)(Common_Sin(t2) * 0.002f);
	//}

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK SoundPlayer::pcmsetposcallback(FMOD_SOUND * sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
{
	/*
	This is useful if the user calls Channel::setPosition and you want to seek your data accordingly.
	*/

	/*
	Will need to check if point is within either buffers
	If so switch to that buffer and start to load the next buffer
	Else pause and start loading from the new point into m_otherBuffer
	*/
	return FMOD_OK;
}

bool SoundPlayer::swapBuffer()
{
	EVALUATEC(m_currentBuffer != nullptr);
	EVALUATEC(m_otherBuffer != nullptr);

	if (m_bufferMutex.try_lock())
	{
		if (m_otherBuffer->isFilled())
		{
			SP::WavBuffer * temp = m_otherBuffer;
			m_otherBuffer = m_currentBuffer;
			m_currentBuffer = temp;
			// do swap code here ##############################
			m_otherBuffer->m_FillIndex = 0; // set other buffer to being unfilled
			m_otherBuffer->m_Index = m_currentBuffer->m_Index + m_currentBuffer->m_dataLength; // set start of buffer to end of other buffer
			EVALUATEC(m_bufferFiller != nullptr);
			m_bufferFiller->setFillBuffer(&m_bufferMutex, m_otherBuffer);
			return true; // exit here
		}
	}
	return false; // exit here
}

void SoundPlayer::setPause(bool state)
{
	EVALUATEC(m_channel != nullptr);
	m_channelMutex.lock();
	m_channel->setPaused(true);
	m_channelMutex.unlock();
}

bool SoundPlayer::getPause()
{
	EVALUATEC(m_channel != nullptr);
	m_channelMutex.lock();
	bool paused;
	m_channel->getPaused(&paused);
	m_channelMutex.unlock();
	return paused;
}

void SoundPlayer::togglePause()
{
	EVALUATEC(m_channel != nullptr);
	m_channelMutex.lock();
	bool paused;
	m_channel->getPaused(&paused);
	m_channel->setPaused(!paused);
	m_channelMutex.unlock();
}
