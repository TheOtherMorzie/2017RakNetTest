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
	TTestBufferFiller * ic = (*interfaceClass);
	ic->setActive(true);
	interfaceMutex->unlock();

	// do the things
	SP::WavBuffer ** buffer = nullptr;
	std::mutex * tbm = nullptr;
	std::mutex ** bufferMutex = nullptr;
	WavFile * file = nullptr;
	WavFile::subChunk * dataSC = nullptr;
	do
	{
		// reset
		buffer = nullptr;
		tbm = nullptr;
		bufferMutex = nullptr;
		file = nullptr;
		dataSC = nullptr;
		// collect
		interfaceMutex->lock();
		buffer = ic->getBuffer();
		tbm = ic->getBufferMutex();
		bufferMutex = &(tbm);
		file = ic->getFile();
		dataSC = ic->getDataSubChunk();
		interfaceMutex->unlock();
	} while ((buffer == nullptr) || (tbm == nullptr) || (bufferMutex == nullptr) || (file == nullptr) || (dataSC == nullptr));

	bool run = true;
	while (run)
	{
		if ((*buffer) != nullptr) // there is a buffer
		{
			interfaceMutex->lock();
			if ((*buffer)->isFilled() == false) // not filled
			{
				// start filling
				(*bufferMutex)->lock();
				SP::WavBuffer * b = (*buffer);
				int fillAmount = b->m_dataLength - b->m_FillIndex;
				memcpy(&b->m_data[b->m_FillIndex], &file->getRawData()[dataSC->index + 4], fillAmount);
				b->m_FillIndex += fillAmount;
				(*bufferMutex)->unlock();
			}
			interfaceMutex->unlock();
		}
	}

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
	EVALUATEC(file != nullptr);
	m_file = file;

	for (size_t i = 0; i < m_file->getDataSubchunkIndexList().size(); i++)
	{
		WavFile::subChunk * sc = &m_file->getDataSubchunkIndexList()[i];
		char idc[5];
		memcpy(idc, m_file->getDataSubchunkIndexList()[i].ID, 4);
		idc[4] = '\0';
		std::string id(idc);
		std::string fmt("fmt ");
		std::string data("data");
		if (id == std::string("fmt "))
		{
			memcpy(&m_format, &m_file->getRawData()[sc->index + 8], 2);			// audio format (AudioFormat)
			memcpy(&m_channelCount, &m_file->getRawData()[sc->index + 10], 2);	// channel count (NumChannels)
			memcpy(&m_playBackRate, &m_file->getRawData()[sc->index + 12], 2);	// sample rate (SampleRate) in samples per second
			memcpy(&m_sampleSize, &m_file->getRawData()[sc->index + 22], 2);	// sample size (BitsPerSample) in bits

			EVALUATEC((m_sampleSize % 8) == 0); // make sure that m_sampe size is a multiple of 8
			m_sampleSize /= 8; // convert bits to bytes
			//m_file->getRawData()[sc->index + 10]; // channel count (NumChannels)
			//m_file->getRawData()[sc->index + 22]; // sample size (BitsPerSample) in bits
			m_fmtSubChunk = sc;
		}
		if (id == std::string("data"))
		{
			m_totalSize = sc->size;
			m_dataSubChunk = sc;
		}
	}
}

int TTestBufferFiller::getBufferSize()
{
	return 2048; // ################# this needs to be changed
}

int TTestBufferFiller::getTotalSize()
{
	return m_totalSize;
}

int TTestBufferFiller::getSampleSize()
{
	return m_sampleSize;
}

int TTestBufferFiller::getChannelCount()
{
	return m_channelCount;
}

int TTestBufferFiller::getPlayBackRate()
{
	return m_playBackRate;
}

int TTestBufferFiller::getFormat()
{
	return m_format;
}

WavFile * TTestBufferFiller::getFile()
{
	return m_file;
}

WavFile::subChunk * TTestBufferFiller::getFmtSubChunk()
{
	return m_fmtSubChunk;
}

WavFile::subChunk * TTestBufferFiller::getDataSubChunk()
{
	return m_dataSubChunk;
}
