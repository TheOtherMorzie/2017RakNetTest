#pragma once

#include <vector>
#include <string>


class WavFile
{
public:

	enum ErrorCodes
	{
		WFEC_NO_ERROR,
		WFEC_FILE_DOES_NOT_EXIST,
		WFEC_FILE_FAILED_TO_OPEN,
		WFEC_FILE_NOT_WAV_TYPE, // wrong file type
		WFEC_FILE_OVERRUN // file loaded more file than exists (probably corrupt file)
	};

	enum FileStates
	{
		WFFS_READY,
		WFFS_NO_FILE_LOADED,
		WFFS_FILE_LOADED_INCORRECTLY
	};

	struct subChunk
	{
		unsigned int index; // points to the ID
		char ID[4];
		unsigned __int32 size; // size of subchunk's data (does not include ID or SIZE) (+4 to get to actual data)
	};

	// actual class

	WavFile();
	~WavFile();

	int loadSoundFile(const std::string & filePath);

	__int32 getSoundDataSize();
	char * getSoundData();
	char * getRawData();
	unsigned int getRawDataLength();

	int getDataSubchunkCount();
	std::vector<WavFile::subChunk> & getDataSubchunkIndexList();

	bool hasData();

	int getFileState();
	const std::string & getFilePath();


protected:

	std::string m_filePath;
	int m_fileState = WavFile::FileStates::WFFS_NO_FILE_LOADED;

	// all data
	char * m_data = nullptr; // owns this
	unsigned int m_dataLength = 0;

	int m_dataSubchunkCount = 0;
	std::vector<WavFile::subChunk> m_dataSubcheunkIndexList;

};

