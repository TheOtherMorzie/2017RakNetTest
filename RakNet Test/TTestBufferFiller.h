#pragma once
#include "TBufferFiller.h"


#include "WavFile.h"

// reference http://soundfile.sapp.org/doc/WaveFormat/

class TTestBufferFiller : public TBufferFiller
{
public:

	static void thread(int * errorCode, TTestBufferFiller ** interfaceClass, std::mutex * interfaceMutex);

	TTestBufferFiller();
	virtual ~TTestBufferFiller();

	void addFile(WavFile * file);

	virtual int getBufferSize();
	virtual int getTotalSize();
	virtual int getSampleSize();
	virtual int getChannelCount();
	virtual int getPlayBackRate();
	virtual int getFormat();

protected:

	WavFile * getFile();
	WavFile::subChunk * getFmtSubChunk();
	WavFile::subChunk * getDataSubChunk();

	WavFile * m_file = nullptr; // does not own this

	WavFile::subChunk * m_fmtSubChunk = nullptr;
	WavFile::subChunk * m_dataSubChunk = nullptr;

	int m_totalSize = 0;
	__int16 m_sampleSize = 0;
	__int16 m_channelCount = 0;
	__int32 m_playBackRate = 0;
	__int16 m_format = 0;


};

