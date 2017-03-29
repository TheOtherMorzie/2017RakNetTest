#include <iostream>

#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>

#define SERVER_PORT 25505
#define CLIENT_PORT 25506
#define IP "125.253.100.212"

int main()
{
	// check for client
	std::cout << "client (y/n) ";
	char cs;
	std::cin >> cs;
	//






	RakNet::RakPeerInterface * peer = RakNet::RakPeerInterface::GetInstance();

	if (tolower(cs) == 'y') // client
	{
		std::cout << "starting client...\n";
		peer->Startup(1, &RakNet::SocketDescriptor::SocketDescriptor(), 1);
		peer->Connect(IP, CLIENT_PORT, 0, 0);

		RakNet::Packet * packet = nullptr;
		while (packet == nullptr)
		{
			packet = peer->Receive();
		}

		if (packet != nullptr)
		{
			if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			{
				std::cout << "connection sucessful\n";
			}
			else
			{
				std::cout << "failed to connect\n";
			}
		}
		else
		{
			std::cout << "packet was nullptr\n";
		}
		
		
	}
	else // server
	{
		std::cout << "starting server...\n";

		RakNet::SocketDescriptor sd(CLIENT_PORT, 0);
		peer->Startup(1, &sd, 1);
		peer->SetMaximumIncomingConnections(4);
	}

	
	system("pause");
	return 0;
}