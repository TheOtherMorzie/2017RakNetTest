#pragma once

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

class SoundPlayer
{

	struct WavBuffer
	{
		WavBuffer(int dataSize) { m_data = new char[dataSize]; m_dataLength = dataSize; };
		~WavBuffer() { if (m_data != nullptr) { delete m_data; } };

		inline bool isFilled() { return (m_FillIndex >= m_dataLength); }; // return if all of m_data is filled with valid data
		inline bool isDone() { return (m_currentSoundPoint >= m_dataLength); }; // return if buffer has been played though entirely

		int m_Index = 0; // position of the start of buffer relative to start of file
		int m_currentSoundPoint = 0; // the index of sample to be played next
		int m_FillIndex = 0; // index of last valid sample in m_data
		int m_dataLength = 0; // size of data
		char* m_data = nullptr;  // raw data
	};

	enum BufferStatBitFlags
	{
		BSBF_OK = 0x00,							// 0000 0000 (not an actual flag, just for easy reading)
		BSBF_STARVED = 0x01,					// 0000 0001
		BSBF_WAITING_ON_OTHER_BUFFER = 0x02		// 0000 0010
	};

public:
	SoundPlayer();
	~SoundPlayer();

	int init(int bufferSizeByte);

	FMOD_RESULT F_CALLBACK pcmreadcallback(FMOD_SOUND* sound, void *data, unsigned int datalen);
	FMOD_RESULT F_CALLBACK pcmsetposcallback(FMOD_SOUND* sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype);

	// will return truw if other buffer is filled
	bool swapBuffer();

protected:


	int m_numberOfChannels = 0;
	int m_sampleIndex; // point along file (0:05 in x 44,100 samples per second means m_sampleIndex = 220,500)
	int m_sampleSize; // size of each sample

	int m_bufferSizeByte = 0; // this needs to be a mutiple of channels multiplied by the sample size
	unsigned char m_bufferState = 0x00;
	WavBuffer * m_currentBuffer = nullptr;
	WavBuffer * m_otherBuffer = nullptr;


};

