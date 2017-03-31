#include "WavFile.h"



WavFile::WavFile()
{
}


WavFile::~WavFile()
{
}

__int32 WavFile::getSoundDataSize()
{
	return (*m_data)[40];
}

char * WavFile::getSoundData()
{
	return (*m_data) + 44;
}

char * WavFile::getRawData()
{
	return *m_data;
}

bool WavFile::hasData()
{
	return (m_data != nullptr);
}
