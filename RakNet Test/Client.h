#pragma once
#include "TBufferFiller.h"

#include <mutex>
#include <string>
#include <vector>

#include <RakNetTypes.h>



class Client : public TBufferFiller
{
public:
	static void thread(int * errorCode, Client ** interfaceClass, std::mutex * interfaceMutex);

	Client();
	virtual ~Client();

	virtual int getBufferSize();
	virtual int getTotalSize();
	virtual __int16 getSampleSize();
	virtual __int16 getChannelCount();

	void connect(std::string ip, int port);

	void addConnection(RakNet::RakPeerInterface * peer);

	bool tick();

	void sendMessage(std::string & message);

protected:

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;

	RakNet::RakPeerInterface * m_peer = nullptr;

};

