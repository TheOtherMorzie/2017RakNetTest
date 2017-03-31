#pragma once
class WavFile
{
public:
	WavFile();
	~WavFile();

	__int32 getSoundDataSize();
	char * getSoundData();
	char * getRawData();

	bool hasData();

protected:

	char ** m_data = nullptr;
	int m_dataLength = 0;

};

