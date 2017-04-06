#pragma once

namespace SP
{
	struct WavBuffer
	{
		WavBuffer(int dataSize) { m_data = new char[dataSize]; m_dataLength = dataSize; };
		~WavBuffer() { if (m_data != nullptr) { delete m_data; } };

		inline bool isFilled() { return (m_FillIndex >= m_dataLength); }; // return if all of m_data is filled with valid data
																		  //inline bool isDone() { return (m_currentSoundPoint >= m_dataLength); }; // return if buffer has been played though entirely (by checking index and length)

		int m_Index = 0; // position of the start of buffer relative to start of file
		int m_FillIndex = 0; // index of last valid sample in m_data
		int m_dataLength = 0; // size of data
		char* m_data = nullptr;  // raw data
		bool m_isDone = true; // to signal if the buffer has been read and should be switched
	};

	enum BufferStatBitFlags
	{
		BSBF_OK = 0x00,							// 0000 0000 (not an actual flag, just for easy reading)
		BSBF_STARVED = 0x01,					// 0000 0001
		BSBF_WAITING_ON_OTHER_BUFFER = 0x02		// 0000 0010
	};
}
