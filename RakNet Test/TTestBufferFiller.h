#pragma once
#include "TBufferFiller.h"


#include "WavFile.h"

class TTestBufferFiller : public TBufferFiller
{
public:

	static void thread(int * errorCode, TTestBufferFiller ** interfaceClass, std::mutex * interfaceMutex);

	TTestBufferFiller();
	virtual ~TTestBufferFiller();

	void addFile(WavFile * file);

	virtual int getBufferSize();
	virtual int getTotalSize();
	virtual __int16 getSampleSize();
	virtual __int16 getChannelCount();

protected:

	WavFile * m_file = nullptr; // does not own this

	int m_totalSize = 0;
	__int16 m_sampleSize = 0;
	__int16 m_channelCount = 0;


};

