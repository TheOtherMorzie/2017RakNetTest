#include "SoundPlayer.h"


#include <cstring> // for low level memory functions (ie. memcpy)
#include <string>


#include "ExceptionC.h"
#include "TBufferFiller.h"

// for retrieveing the SoundPlayer
#include "SoundPlayerHandler.h"


FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND* /*sound*/, void *data, unsigned int datalen)
{
	static float  t1 = 0, t2 = 0;        // time
	static float  v1 = 0, v2 = 0;        // velocity
	signed short *stereo16bitbuffer = (signed short *)data;

	for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
	{
		*stereo16bitbuffer++ = (signed short)(sin(t1) * 32767.0f);    // left channel
		*stereo16bitbuffer++ = (signed short)(sin(t2) * 32767.0f);    // right channel

		t1 += 0.01f + v1;
		t2 += 0.0142f + v2;
		v1 += (float)(sin(t1) * 0.002f);
		v2 += (float)(sin(t2) * 0.002f);
	}

	printf("pcmReadCallBack");
	//system("pause");

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND* /*sound*/, int /*subsound*/, unsigned int /*position*/, FMOD_TIMEUNIT /*postype*/)
{
	/*
	This is useful if the user calls Channel::setPosition and you want to seek your data accordingly.
	*/
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK pcmreadcallbackC(FMOD_SOUND * sound, void * data, unsigned int datalen)
{
	printf("######### pcmreadcallback");
	FMOD::Sound *snd = (FMOD::Sound *)sound;
	SoundPlayer * sp = nullptr;
	{
		std::mutex * sphm = &SoundPlayerHandler::getHandlerMutex();
		sphm->lock();
		printf("SoundPlayer[Callback](53): soundPlayerHandler locked!\n");
		sp = SoundPlayerHandler::getHandler().getSoundPlayer(snd);
		sphm->unlock();
		printf("SoundPlayer[Callback](56): soundPlayerHandler unlocked!\n");
	}
	if (sp == nullptr)
	{
		printf("#########failed to get soundPlayer!\n");
		return FMOD_OK;
	}
	else
	{
		printf("#########got soundplayer!\n");
	}

	SP::WavBuffer * currentBuffer = sp->getCurrentBuffer();

	EVALUATEC(currentBuffer != nullptr);


	if (currentBuffer->m_isDone) // buffers have not been switched yet
	{
		if (!sp->swapBuffer()) // buffer failed to swap (other buffer is not filled)
		{
			sp->setBufferBit(SP::BSBF_WAITING_ON_OTHER_BUFFER, true);
			// to stop it looping over the last loaded buffer
			sp->setPause(true);
			while (sp->swapBuffer() == false)
			{
				// give the program a chance to swap the buffers and to avoid locking up the cpu
				printf("######### Waiting on other buffer to be filled\n");
				std::chrono::milliseconds timespan(100);
				std::this_thread::sleep_for(timespan);
			}
			sp->setBufferBit(SP::BSBF_WAITING_ON_OTHER_BUFFER, false);
			sp->setPause(false);
		}
	}

	// the current buffer should not be done
	EVALUATEC(currentBuffer->m_isDone);

	// load currentBuffer into data


	// this will loop though data allowing the audio buffer to be set
	signed short *stereo16bitbuffer = (signed short *)data;
	std::mutex * bm = sp->getBufferMutex();
	EVALUATEC(bm != nullptr);
	bool done = false;
	while (done == false)
	{
		if (bm->try_lock())
		{
			printf("######### Filling FMOD buffer\n");
			printf("SoundPlayer[Callback](107): buffer mutex locked!\n");
			//printf("loading sound");
			int sampleCount = datalen >> 1;
			for (unsigned int count = 0; count < (datalen >> 1); count++)     // >>2 = 16bit stereo (4 bytes per sample)
			{
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel
				char c1[2];
				char c2[2];
				memcpy(&c1, &(currentBuffer->m_data[count * 2]), sp->getSampleSize()); // left channel
				memcpy(&c2, &(currentBuffer->m_data[(count * 2) + 1]), sp->getSampleSize()); // right channel
				memcpy(&(*stereo16bitbuffer++), &(currentBuffer->m_data[count * 2]), sp->getSampleSize()); // left channel
				memcpy(&(*stereo16bitbuffer++), &(currentBuffer->m_data[(count * 2) + 1]), sp->getSampleSize()); // right channel

				//memset(&(*stereo16bitbuffer++), 0, sp->getSampleSize()); // set data to null
				//memset(&(*stereo16bitbuffer++), 0, sp->getSampleSize()); // set data to null

				sp->getSampleIndex()++;
			}
			currentBuffer->m_isDone = true;
			sp->swapBuffer(); // swap buffers so that the filler can fill otherBuffer


			(*bm).unlock();
			printf("SoundPlayer[Callback](123): buffer mutex unlocked!\n");
			done = true;
		}
		else
		{
			printf("######### Waiting on SoundPlayer Mutex...\n");
			std::chrono::milliseconds timespan(100);
			std::this_thread::sleep_for(timespan);
		}
	}

	printf("######### Sound Player callback done\n");
	sp->setPause(false);


	// old
	/*
	bool callBackDone = false;
	while (!callBackDone)
	{
		if (!currentBuffer->m_isDone) // play sample from current buffer
		{
			// play sample

			// this will loop though data allowing the audio buffer to be set
			signed short *stereo16bitbuffer = (signed short *)data;
			for (unsigned int count = 0; count < (datalen >> 2); count++)     // >>2 = 16bit stereo (4 bytes per sample)
			{
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
				//*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel
				memcpy(&(*stereo16bitbuffer++), &(currentBuffer->m_data[count * 2]), sampleSize); // left channel
				memcpy(&(*stereo16bitbuffer++), &(currentBuffer->m_data[(count * 2) + 1]), sampleSize); // right channel
			}

			//
			sampleIndex++; // has played a sample and should move on
			callBackDone = true;
		}
		else // switch buffers
		{
			bool otherFilled = sp->swapBuffer();
			if (!otherFilled) // still on same buffer
			{
				sp->setBufferBit(SP::BSBF_WAITING_ON_OTHER_BUFFER, true);
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
		*stereo16bitbuffer++ = (signed short)(Common_Sin(t1) * 32767.0f);    // left channel
		*stereo16bitbuffer++ = (signed short)(Common_Sin(t2) * 32767.0f);    // right channel

		t1 += 0.01f + v1;
		t2 += 0.0142f + v2;
		v1 += (float)(Common_Sin(t1) * 0.002f);
		v2 += (float)(Common_Sin(t2) * 0.002f);
	}
	*/

	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK pcmsetposcallbackC(FMOD_SOUND * sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype)
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


SoundPlayer::SoundPlayer()
{
	m_bufferMutex = new std::mutex;
}


SoundPlayer::~SoundPlayer()
{
	FMOD_RESULT result;
	/*
	Shut down
	*/
	result = m_sound->release();
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
	result = m_system->close();
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
	result = m_system->release();
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
	if (m_bufferMutex != nullptr)
	{
		delete m_bufferMutex;
	}
}

int SoundPlayer::init(TBufferFiller & bufferFiller)
{
	m_bufferSizeByte = bufferFiller.getBufferSize();
	m_numberOfChannels = bufferFiller.getChannelCount();
	m_sampleSize = bufferFiller.getSampleSize();
	m_playbackRate = bufferFiller.getPlayBackRate();
	m_totalFileSize = bufferFiller.getTotalSize();

	bufferFiller.setFillBuffer(m_bufferMutex, &m_otherBuffer);

	m_currentBuffer = new SP::WavBuffer(m_bufferSizeByte);
	m_otherBuffer = new SP::WavBuffer(m_bufferSizeByte);

	// setup fmod objects

	FMOD_RESULT             result;
	//FMOD_MODE               mode = FMOD_OPENUSER | FMOD_LOOP_NORMAL;
	FMOD_MODE               mode = FMOD_OPENUSER | FMOD_LOOP_NORMAL | FMOD_CREATESTREAM;
	FMOD_CREATESOUNDEXINFO  exinfo;
	unsigned int            version;
	void                   *extradriverdata = 0;

	//Common_Init(&extradriverdata);

	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&m_system);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return result;
	}

	result = m_system->getVersion(&version);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return result;
	}

	if (version < FMOD_VERSION)
	{
		printf("FMOD lib version %d doesn't match header version %d", version, FMOD_VERSION);
	}

	result = m_system->init(512, FMOD_INIT_NORMAL, extradriverdata);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return result;
	}

	// set stream info
	if (true)
	{
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);					/* Required. */
		exinfo.numchannels = m_numberOfChannels;						/* Number of channels in the sound. */
		exinfo.defaultfrequency = m_playbackRate;						/* Default playback rate of sound. */
		exinfo.decodebuffersize = m_bufferSizeByte;						/* Chunk size of stream update in samples. This will be the amount of data passed to the user callback. */
																		//exinfo.decodebuffersize = 2048; /* 44100 / 5 = 8820 */		/* Chunk size of stream update in samples. This will be the amount of data passed to the user callback. */
		exinfo.length = m_totalFileSize;								/* Length of PCM data in bytes of whole song (for Sound::getLength) */
		FMOD_SOUND_FORMAT f;
		if (bufferFiller.getFormat() == 1)
		{
			switch (bufferFiller.getSampleSize())
			{
			case 1:
				f = FMOD_SOUND_FORMAT_PCM8;
				break;
			case 2:
				f = FMOD_SOUND_FORMAT_PCM16;
				break;
			case 3:
				f = FMOD_SOUND_FORMAT_PCM24;
				break;
			case 4:
				f = FMOD_SOUND_FORMAT_PCM32;
				break;
			default:
				std::string s("unknown sample size: ");
				s += std::to_string(bufferFiller.getSampleSize());
				char * c = new char[s.size()];
				strcpy(c, s.c_str());
				THROWC(c);
				break;
			}
		}
		else
		{
			std::string s("unknown file format: ");
			s += std::to_string(bufferFiller.getFormat());
			char * c = new char[s.size()];
			strcpy(c, s.c_str());
			THROWC(c);
		}
		exinfo.format = f;												/* Data format of sound. */
		exinfo.pcmreadcallback = pcmreadcallbackC;		/* User callback for reading. */
		exinfo.pcmsetposcallback = pcmsetposcallbackC;	/* User callback for seeking. */
	}

	// for debugging
	if (false)
	{
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);												/* Required. */
		exinfo.numchannels = 2;																		/* Number of channels in the sound. */
		exinfo.defaultfrequency = 44100;															/* Default playback rate of sound. */
		exinfo.decodebuffersize = 44100;															/* Chunk size of stream update in samples. This will be the amount of data passed to the user callback. */
		exinfo.length = exinfo.defaultfrequency * exinfo.numchannels * sizeof(signed short) * 5;	/* Length of PCM data in bytes of whole song (for Sound::getLength) */
		exinfo.format = FMOD_SOUND_FORMAT_PCM16;													/* Data format of sound. */
		exinfo.pcmreadcallback = pcmreadcallbackC;													/* User callback for reading. */
		exinfo.pcmsetposcallback = pcmsetposcallbackC;												/* User callback for seeking. */
	}

	result = m_system->createSound(0, mode, &exinfo, &m_sound);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return result;
	}

	result = m_system->playSound(m_sound, 0, false, &m_channel);
	if (result != FMOD_OK)
	{
		printf("FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result));
		return result;
	}

	std::mutex * sphm = &SoundPlayerHandler::getHandlerMutex();
	sphm->lock();
	printf("SoundPlayer(369): soundPlayerHandler locked!\n");
	SoundPlayerHandler::getHandler().addSoundSoundPlayerPair(m_sound, this);
	sphm->unlock();
	printf("SoundPlayer(369): soundPlayerHandler unlocked!\n");

	return 0;
}

bool SoundPlayer::swapBuffer()
{
	EVALUATEC(m_currentBuffer != nullptr);
	EVALUATEC(m_otherBuffer != nullptr);

	if (m_bufferMutex->try_lock())
	{
		printf("SoundPlayer(384): buffer mutex locked!\n");
		if (m_otherBuffer->isFilled())
		{
			printf("######### Swaping buffers");
			SP::WavBuffer * temp = m_otherBuffer;
			m_otherBuffer = m_currentBuffer;
			m_currentBuffer = temp;
			// do swap code here ##############################
			m_otherBuffer->m_FillIndex = 0; // set other buffer to being unfilled
			m_otherBuffer->m_Index = m_currentBuffer->m_Index + m_currentBuffer->m_dataLength; // set start of buffer to end of other buffer
			m_bufferMutex->unlock();
			printf("SoundPlayer(394): buffer mutex unlocked!\n");
			return true; // exit here
		}
		m_bufferMutex->unlock();
		printf("SoundPlayer(398): buffer mutex unlocked!\n");
	}
	return false; // exit here
}

void SoundPlayer::setPause(bool state)
{
	EVALUATEC(m_channel != nullptr);
	m_channelMutex.lock();
	printf("SoundPlayer(407): channel locked!\n");
	m_channel->setPaused(true);
	m_channelMutex.unlock();
	printf("SoundPlayer(410): channel unlocked!\n");
}

bool SoundPlayer::getPause()
{
	EVALUATEC(m_channel != nullptr);
	m_channelMutex.lock();
	printf("SoundPlayer(417): channel locked!\n");
	bool paused;
	m_channel->getPaused(&paused);
	m_channelMutex.unlock();
	printf("SoundPlayer(421): channel unlocked!\n");
	return paused;
}

void SoundPlayer::togglePause()
{
	EVALUATEC(m_channel != nullptr);
	m_channelMutex.lock();
	printf("SoundPlayer(429): channel locked!\n");
	bool paused;
	m_channel->getPaused(&paused);
	m_channel->setPaused(!paused);
	m_channelMutex.unlock();
	printf("SoundPlayer(434): channel unlocked!\n");
}

SP::WavBuffer * SoundPlayer::getCurrentBuffer()
{
	return m_currentBuffer;
}

SP::WavBuffer * SoundPlayer::getOtherBuffer()
{
	return m_otherBuffer;
}

FMOD::Sound * SoundPlayer::getSound()
{
	return m_sound;
}

FMOD::Channel * SoundPlayer::getChannel()
{
	return m_channel;
}

FMOD::System * SoundPlayer::getSystem()
{
	return m_system;
}

void SoundPlayer::setBufferBit(int bit, bool state)
{
	if (state)
	{
		m_bufferState |= bit;
	}
	else
	{
		m_bufferState &= ~bit;
	}
}

int SoundPlayer::getSampleSize()
{
	return m_sampleSize;
}

int & SoundPlayer::getSampleIndex()
{
	return m_sampleIndex;
}

std::mutex * SoundPlayer::getBufferMutex()
{
	return m_bufferMutex;
}
