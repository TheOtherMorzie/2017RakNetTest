#include "TTestBufferFiller.h"

#include "ExceptionC.h"



void TTestBufferFiller::thread(int * errorCode, TTestBufferFiller ** interfaceClass, std::mutex * interfaceMutex)
{
	if (interfaceMutex == nullptr)
	{
		(*errorCode) = 1;
		THROWC("interfaceMutex was nullptr");
		return;
	}

	// start setting up client
	interfaceMutex->lock();
	(*interfaceClass) = new TTestBufferFiller();
	TTestBufferFiller * ic = (*interfaceClass);
	ic->setActive(true);

	ic->setActive(false);
}

TTestBufferFiller::TTestBufferFiller()
{
}


TTestBufferFiller::~TTestBufferFiller()
{
}

void TTestBufferFiller::addFile(WavFile * file)
{
	m_file = file;

	for (size_t i = 0; i < m_file->getDataSubchunkIndexList().size(); i++)
	{
		WavFile::subChunk * sc = &m_file->getDataSubchunkIndexList()[i];
		char idc[5];
		strcpy(idc, m_file->getDataSubchunkIndexList()[i].ID);
		idc[4] = '\0';
		std::string id(idc);
		std::string fmt("fmt ");
		std::string data("data");
		if (id == std::string("fmt "))
		{
			memcpy(&m_channelCount, &m_file->getRawData()[sc->index + 10], 2);// channel count (NumChannels)
			memcpy(&m_sampleSize, &m_file->getRawData()[sc->index + 22], 2);// sample size (BitsPerSample) in bits
			EVALUATEC((m_sampleSize % 8) == 0); // make sure that m_sampe size is a multiple of 8
			m_sampleSize /= 8; // convert bits to bytes
			//m_file->getRawData()[sc->index + 10]; // channel count (NumChannels)
			//m_file->getRawData()[sc->index + 22]; // sample size (BitsPerSample) in bits
		}
		if (id == std::string("data"))
		{
			m_totalSize = sc->size;
		}
	}
}

int TTestBufferFiller::getBufferSize()
{
	return 2048; // ################# this needs to be changed
}

int TTestBufferFiller::getTotalSize()
{
	EVALUATEC(m_file != nullptr);
	return m_totalSize;
}

__int16 TTestBufferFiller::getSampleSize()
{
	return 0;
}

__int16 TTestBufferFiller::getChannelCount()
{
	return 0;
}
