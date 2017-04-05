#include "SoundPlayer.h"

#include "ExceptionC.h"


SoundPlayer::SoundPlayer()
{
}


SoundPlayer::~SoundPlayer()
{
}

int SoundPlayer::init(int bufferSizeByte)
{
	m_bufferSizeByte = bufferSizeByte;

	m_currentBuffer = new WavBuffer(m_bufferSizeByte);
	m_otherBuffer = new WavBuffer(m_bufferSizeByte);


	return 0;
}

FMOD_RESULT F_CALLBACK SoundPlayer::pcmreadcallback(FMOD_SOUND * sound, void * data, unsigned int datalen)
{
	EVALUATEC(m_currentBuffer != nullptr);
	EVALUATEC(m_otherBuffer != nullptr);


	bool callBackDone = false;
	while (!callBackDone)
	{
		if (!m_currentBuffer->isDone()) // play sample from current buffer
		{
			// play sample

			signed short *stereo16bitbuffer = (signed short *)data;
			for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
			{
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel
				*stereo16bitbuffer++;
				*stereo16bitbuffer++;
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
				
				m_bufferState |= BSBF_WAITING_ON_OTHER_BUFFER;
				callBackDone = true;
			}
			// won't exit while loop so that the new buffer can play it's sample
		}
	}


	// ############################# example code
	static float  t1 = 0, t2 = 0;        // time
	static float  v1 = 0, v2 = 0;        // velocity
	signed short *stereo16bitbuffer = (signed short *)data;

	for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
	{
		//*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
		//*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel
		*stereo16bitbuffer++;
		*stereo16bitbuffer++;

		t1 += 0.01f + v1;
		t2 += 0.0142f + v2;
		//v1 += (float)(Common_Sin(t1) * 0.002f);
		//v2 += (float)(Common_Sin(t2) * 0.002f);
	}

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

	if (m_otherBuffer->isFilled())
	{
		WavBuffer * temp = m_otherBuffer;
		m_otherBuffer = m_currentBuffer;
		m_currentBuffer = temp;
	}
	// do swap code here ##############################
}
