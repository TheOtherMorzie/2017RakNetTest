#include "Client.h"

#include <iostream>
#include <vector>
#include <string>

#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <NatPunchthroughClient.h>

#include "RakNetHelperFunctions.h"

#include "ExceptionC.h"

#define SERVER_PORT 25505
#define CLIENT_PORT 25506



void Client::thread(int * errorCode, Client ** interfaceClass, std::mutex * interfaceMutex)
{
	if (interfaceMutex == nullptr)
	{
		THROWC("interfaceMutex was nullptr");
		(*errorCode) = 1;
		return;
	}

	// start setting up client
	interfaceMutex->lock();
	(*interfaceClass) = new Client();

	std::vector<char*> ipList = { "localhost","10.10.22.20", "122.99.85.34" };

	RakNet::RakPeerInterface * peer = RakNet::RakPeerInterface::GetInstance();

	std::string ip;
	std::cout << "select ip to connect to\n\n";
	std::cout << "[00] - custom\n\n";
	for (size_t i = 0; i < ipList.size(); i++)
	{
		std::cout << "[" << i + 1 << "] - " << ipList[i] << "\n";
	}
	std::cout << " input number:";
	int selection = -1;
	std::cin.clear();
	std::cin.ignore(99999999999999, '\n');
	std::cin >> selection;
	while (selection < 0 || selection > ipList.size())
	{
		std::cout << "invalid selection\ninput number:";
		std::cin.clear();
		std::cin.ignore(99999999999999, '\n');
		std::cin >> selection;
	}
	if (selection == 0)
	{
		printf("\ninput ip:");
		std::cin >> ip;
	}
	else
	{
		ip = ipList[selection - 1];
	}

	std::cout << "starting client...\n";
	peer->Startup(1, &RakNet::SocketDescriptor::SocketDescriptor(), 1);
	// attach nat punchthrough plugin
	RakNet::NatPunchthroughClient nptc;
	peer->AttachPlugin(&nptc);
	peer->Connect(ip.c_str(), CLIENT_PORT, 0, 0);

	RakNet::Packet * packet = nullptr;
	while (packet == nullptr)
	{
		packet = peer->Receive();
	}

	std::cout << "packet recieved!\n";
	if (packet != nullptr)
	{
		if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
		{
			peer->Connect("natpunch.jenkinssoftware.com", 61111, 0, 0); // connect to server for nat punchthrough
		}

		std::string packetType = RakNetHelpers::getPacketType(*packet);
		std::cout << packetType << "\n";
	}
	else
	{
		std::cout << "packet was nullptr\n";
	}

	(*interfaceClass)->addConnection(peer);

	// finished setting up client
	interfaceMutex->unlock();

	bool closeThread = false;
	while (closeThread == false)
	{
		closeThread = (*interfaceClass)->tick();
	}

	peer->Shutdown(10);
}

Client::Client()
{
}


Client::~Client()
{
}

void Client::connect(std::string ip, int port)
{
}

void Client::addConnection(RakNet::RakPeerInterface * peer)
{
	if (m_peer != nullptr)
	{
		m_peer->Shutdown(10);
	}
	m_peer = peer;
}

bool Client::tick()
{
	m_messagesMutex.lock();
	if (m_messages.size() > 0)
	{
		for (size_t i = 0; i < m_messages.size(); i++)
		{
			if (m_peer != nullptr)
			{
				m_messages[i] = ((char)ID_USER_PACKET_ENUM) + m_messages[i];
				m_peer->Send(m_messages[i].c_str(), m_messages[i].length() + 1, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
			}
			else
			{
				THROWC("client has no peer");
			}
		}
		m_messages.clear();
	}
	m_messagesMutex.unlock();
	return false;
}

void Client::sendMessage(std::string & message)
{
	m_messagesMutex.lock();
	m_messages.push_back(message);
	m_messagesMutex.unlock();
}
