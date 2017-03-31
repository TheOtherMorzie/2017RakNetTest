#include "Server.h"

#include <iostream>
#include <vector>
#include <string>

#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <NatPunchthroughClient.h>

#include "RakNetHelperFunctions.h"

#define SERVER_PORT 25505
#define CLIENT_PORT 25506

void Server::thread(int * errorCode, Server ** interfaceClass, std::mutex * interfaceMutex)
{
	RakNet::RakPeerInterface * peer = RakNet::RakPeerInterface::GetInstance();

	std::cout << "starting server...\n";

	RakNet::SocketDescriptor sd(CLIENT_PORT, 0);
	peer->Startup(1, &sd, 1);
	peer->SetMaximumIncomingConnections(4);

	while (true)
	{
		RakNet::Packet *packet;
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			std::cout << "###start\n";
			std::cout << "packet recieved!\n";
			if (packet != nullptr)
			{
				// print packet type
				RakNetHelpers::getPacketType(*packet);
				std::string packetType = RakNetHelpers::getPacketType(*packet);
				std::cout << packetType << "\n";

				// print packet data
				if (packet->length > 1) // has data (first byte is for type enum)
				{
					std::string packetData = reinterpret_cast<char*>(packet->data);
					packetData.erase(0, 1);
					std::cout << "data[" << packet->length - 1 << "]:" << packetData << "\n";
					std::cout << "byteData:";
					for (int i = 0; i < packet->length - 1; i++)
					{
						std::cout << "[" << (int)packet->data[i + 1] << "](" << packet->data[i] << ")";
					}
					std::cout << "\n";

				}
			}
			else
			{
				std::cout << "packet was nullptr\n";
			}
			std::cout << "###end\n\n";
		}
	}
	peer->Shutdown(10);
}

Server::Server()
{
}


Server::~Server()
{
}
