#pragma once
#include "ThreadBase.h"

#include <mutex>
#include <string>
#include <vector>

#include <RakNetTypes.h>

class Client : ThreadBase
{
public:
	static void thread(int * errorCode, Client ** interfaceClass, std::mutex * interfaceMutex);

	Client();
	~Client();

	void connect(std::string ip, int port);

	void addConnection(RakNet::RakPeerInterface * peer);

	bool tick();

	void sendMessage(std::string & message);

protected:

	std::mutex m_messagesMutex;
	std::vector<std::string> m_messages;

	RakNet::RakPeerInterface * m_peer = nullptr;

};

