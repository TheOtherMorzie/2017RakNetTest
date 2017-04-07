#include "WavFile.h"

#include <fstream>

#include <sys\stat.h>

bool fileExists(const std::string & filePath)
{
	struct stat buf;
	if (stat(filePath.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

WavFile::WavFile()
{
}


WavFile::~WavFile()
{
	if (m_data != nullptr)
	{
		delete m_data;
	}
}

int WavFile::loadSoundFile(const std::string & filePath)
{
	if (!fileExists(filePath))
	{
		return WFEC_FILE_DOES_NOT_EXIST;
	}
	m_filePath = filePath;
	std::ifstream file;
	file.open(filePath, std::ios::in | std::ios::binary | std::ios::ate); // io::ate for checking the file size
	if (!file.is_open())
	{
		return WFEC_FILE_FAILED_TO_OPEN;
	}

	m_dataLength = file.tellg();
	file.seekg(0); // move to start of file
	// create data ready for storage
	if (m_data != nullptr)
	{
		delete m_data;
	}
	m_data = new char[m_dataLength];
	file.read(m_data, m_dataLength);

	// check RIFF file type
	file.seekg(8);
	char fileType[4];
	char wave[4] = { 'W','A','V','E' };
	file.read(fileType, 4);
	if (!strcmp(fileType, wave))
	{
		m_fileState = WavFile::FileStates::WFFS_FILE_LOADED_INCORRECTLY;
		return WavFile::ErrorCodes::WFEC_FILE_NOT_WAV_TYPE;
	}

	// move to start of file
	file.seekg(0);

	// check how many chunks there are
	std::streampos cursor;
	cursor = 12; // this will include the "fmt" sub-chunk
	while (cursor < m_dataLength)
	{
		// move to position
		file.seekg(cursor);

		subChunk sc;

		// set index of sub-chunk
		sc.index = cursor;

		// read ID
		file.read(sc.ID, 4); // makes sure it reads all the data

		// read size
		char size[4];
		file.read(size, 4); // makes sure it reads all the data
		//sc.size = (unsigned __int32)size;
		memcpy(&sc.size, &size, 4); // binary copy

		// add sub-chunk to list
		m_dataSubcheunkIndexList.push_back(sc);

		cursor += 8/*id and size byte count*/ + sc.size; // skip over data
	}
	if (cursor > m_dataLength)
	{
		m_fileState = WavFile::FileStates::WFFS_FILE_LOADED_INCORRECTLY;
		return WavFile::ErrorCodes::WFEC_FILE_OVERRUN;
	}
	
	// we're done here
	file.close();

	m_fileState = WavFile::FileStates::WFFS_READY;
	return WFEC_NO_ERROR;
}

__int32 WavFile::getSoundDataSize()
{
	return m_data[40];
}

char * WavFile::getSoundData()
{
	return m_data + 36;
}

char * WavFile::getRawData()
{
	return m_data;
}

int WavFile::getDataSubchunkCount()
{
	return m_dataSubchunkCount;
}

std::vector<WavFile::subChunk>& WavFile::getDataSubchunkIndexList()
{
	return m_dataSubcheunkIndexList;
}

bool WavFile::hasData()
{
	return (m_data != nullptr);
}

int WavFile::getFileState()
{
	return m_fileState;
}

const std::string & WavFile::getFilePath()
{
	return m_filePath;
}
