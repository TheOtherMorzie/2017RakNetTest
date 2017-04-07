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

	virtual int getBufferSize();		// ############## to be implemented
	virtual int getTotalSize();			// ############## to be implemented
	virtual int getSampleSize();		// ############## to be implemented
	virtual int getChannelCount();		// ############## to be implemented
	virtual int getPlayBackRate();		// ############## to be implemented
	virtual int getFormat();			// ############## to be implemented

	void connect(std::string ip, int port);

	void addConnection(RakNet::RakPeerInterface * peer);

	bool tick();

	void sendMessage(std::string & message);

protected:

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;

	RakNet::RakPeerInterface * m_peer = nullptr;

};

