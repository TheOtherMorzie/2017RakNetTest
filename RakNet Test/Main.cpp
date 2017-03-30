#include <iostream>
#include <vector>
#include <string>

#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <NatPunchthroughClient.h>

#define SERVER_PORT 25505
#define CLIENT_PORT 25506
#define IP "122.99.85.34"


std::string getPacketType(const RakNet::Packet & packet)
{
	std::string rp = "[blank]";

	switch (packet.data[0])
	{
	case ID_CONNECTED_PING:
		rp = "connected ping";
		break;
	case ID_UNCONNECTED_PING:
		rp = "unconnected ping";
		break;
	case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
		rp = "unconnected ping open connections";
		break;
	case ID_CONNECTED_PONG:
		rp = "connected pong";
		break;
	case ID_DETECT_LOST_CONNECTIONS:
		rp = "detect lost connections";
		break;
	case ID_OPEN_CONNECTION_REQUEST_1:
		rp = "open connection request 1";
		break;
	case ID_OPEN_CONNECTION_REPLY_1:
		rp = "open connection reply 1";
		break;
	case ID_OPEN_CONNECTION_REQUEST_2:
		rp = "open connection request 2";
		break;
	case ID_OPEN_CONNECTION_REPLY_2:
		rp = "open connection reply 2";
		break;
	case ID_CONNECTION_REQUEST:
		rp = "connection request recieved";
		break;
	case ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY:
		rp = "remote system requires public key";
		break;
	case ID_OUR_SYSTEM_REQUIRES_SECURITY:
		rp = "our system requires security";
		break;
	case ID_PUBLIC_KEY_MISMATCH:
		rp = "public key mismatch";
		break;
	case ID_OUT_OF_BAND_INTERNAL:
		rp = "out of band internal";
		break;
	case ID_SND_RECEIPT_ACKED:
		rp = "SND receipt acked";
		break;
	case ID_SND_RECEIPT_LOSS:
		rp = "SND recipt loss";
		break;
	case ID_CONNECTION_REQUEST_ACCEPTED:
		rp = "connection sucessful";
		break;
	case ID_CONNECTION_ATTEMPT_FAILED:
		rp = "failed to connect";
		break;
	case ID_ALREADY_CONNECTED:
		rp = "already connected";
		break;
	case ID_NEW_INCOMING_CONNECTION:
		rp = "new incomming connection";
		break;
	case ID_NO_FREE_INCOMING_CONNECTIONS:
		rp = "no free incomming connections";
		break;
	case ID_DISCONNECTION_NOTIFICATION:
		rp = "disconnected";
		break;
	case ID_CONNECTION_LOST:
		rp = "connection lost";
		break;
	case ID_CONNECTION_BANNED:
		rp = "connection banned";
		break;
	case ID_INVALID_PASSWORD:
		rp = "invalid password";
		break;
	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
		rp = "ID_INCOMPATIBLE_PROTOCOL_VERSION";
		break;
	case ID_IP_RECENTLY_CONNECTED:
		rp = "ID_IP_RECENTLY_CONNECTED";
		break;
	case ID_TIMESTAMP:
		rp = "timestamp";
		break;
	case ID_UNCONNECTED_PONG:
		rp = "ID_UNCONNECTED_PONG";
		break;
	case ID_ADVERTISE_SYSTEM:
		rp = "ID_ADVERTISE_SYSTEM";
		break;
	case ID_DOWNLOAD_PROGRESS:
		rp = "download progress";
		break;
	case ID_REMOTE_DISCONNECTION_NOTIFICATION:
		rp = "remote disconnet";
		break;
	case ID_REMOTE_CONNECTION_LOST:
		rp = "remote connection lost";
		break;
	case ID_REMOTE_NEW_INCOMING_CONNECTION:
		rp = "remote new incomming connection";
		break;
	case ID_FILE_LIST_TRANSFER_HEADER:
		rp = "ID_FILE_LIST_TRANSFER_HEADER";
		break;
	case ID_FILE_LIST_TRANSFER_FILE:
		rp = "ID_FILE_LIST_TRANSFER_FILE";
		break;
	case ID_FILE_LIST_REFERENCE_PUSH_ACK:
		rp = "ID_FILE_LIST_REFERENCE_PUSH_ACK";
		break;
	case ID_DDT_DOWNLOAD_REQUEST:
		rp = "ddt download request";
		break;
	case ID_TRANSPORT_STRING:
		rp = "ID_TRANSPORT_STRING";
		break;
	case ID_REPLICA_MANAGER_CONSTRUCTION:
		rp = "ID_REPLICA_MANAGER_CONSTRUCTION";
		break;
	case ID_REPLICA_MANAGER_SCOPE_CHANGE:
		rp = "ID_REPLICA_MANAGER_SCOPE_CHANGE";
		break;
	case ID_REPLICA_MANAGER_SERIALIZE:
		rp = "ID_REPLICA_MANAGER_SERIALIZE";
		break;
	case ID_REPLICA_MANAGER_DOWNLOAD_STARTED:
		rp = "ID_REPLICA_MANAGER_DOWNLOAD_STARTED";
		break;
	case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:
		rp = "ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE";
		break;
	case ID_RAKVOICE_OPEN_CHANNEL_REQUEST:
		rp = "ID_RAKVOICE_OPEN_CHANNEL_REQUEST";
		break;
	case ID_RAKVOICE_OPEN_CHANNEL_REPLY:
		rp = "ID_RAKVOICE_OPEN_CHANNEL_REPLY";
		break;
	case ID_RAKVOICE_CLOSE_CHANNEL:
		rp = "ID_RAKVOICE_CLOSE_CHANNEL";
		break;
	case ID_RAKVOICE_DATA: // ##################
		rp = "ID_RAKVOICE_DATA";
		break;
	case ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE:
		rp = "ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE";
		break;
	case ID_AUTOPATCHER_CREATION_LIST:
		rp = "ID_AUTOPATCHER_CREATION_LIST";
		break;
	case ID_AUTOPATCHER_DELETION_LIST:
		rp = "ID_AUTOPATCHER_DELETION_LIST";
		break;
	case ID_AUTOPATCHER_GET_PATCH:
		rp = "ID_AUTOPATCHER_GET_PATCH";
		break;
	case ID_AUTOPATCHER_PATCH_LIST:
		rp = "ID_AUTOPATCHER_PATCH_LIST";
		break;
	case ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR:
		rp = "ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR";
		break;
	case ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES:
		rp = "ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES";
		break;
	case ID_AUTOPATCHER_FINISHED_INTERNAL:
		rp = "ID_AUTOPATCHER_FINISHED_INTERNAL";
		break;
	case ID_AUTOPATCHER_FINISHED:
		rp = "ID_AUTOPATCHER_FINISHED";
		break;
	case ID_AUTOPATCHER_RESTART_APPLICATION:
		rp = "ID_AUTOPATCHER_RESTART_APPLICATION";
		break;
	case ID_NAT_PUNCHTHROUGH_REQUEST:
		rp = "NAT punchthrough request";
		break;
	case ID_NAT_CONNECT_AT_TIME:
		rp = "ID_NAT_CONNECT_AT_TIME";
		break;
	case ID_NAT_GET_MOST_RECENT_PORT:
		rp = "ID_NAT_GET_MOST_RECENT_PORT";
		break;
	case ID_NAT_CLIENT_READY:
		rp = "ID_NAT_CLIENT_READY";
		break;
	case ID_NAT_TARGET_NOT_CONNECTED:
		rp = "ID_NAT_TARGET_NOT_CONNECTED";
		break;
	case ID_NAT_TARGET_UNRESPONSIVE:
		rp = "ID_NAT_TARGET_UNRESPONSIVE";
		break;
	case ID_NAT_CONNECTION_TO_TARGET_LOST:
		rp = "ID_NAT_CONNECTION_TO_TARGET_LOST";
		break;
	case ID_NAT_ALREADY_IN_PROGRESS:
		rp = "ID_NAT_ALREADY_IN_PROGRESS";
		break;
	case ID_NAT_PUNCHTHROUGH_FAILED:
		rp = "NAT punchthrough failed";
		break;
	case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
		rp = "NAT punchthrough succeeded";
		break;
	case ID_READY_EVENT_SET:
		rp = "ID_READY_EVENT_SET";
		break;
	case ID_READY_EVENT_UNSET:
		rp = "ID_READY_EVENT_UNSET";
		break;
	case ID_READY_EVENT_ALL_SET:
		rp = "ID_READY_EVENT_ALL_SET";
		break;
	case ID_READY_EVENT_QUERY:
		rp = "ID_READY_EVENT_QUERY";
		break;
	case ID_LOBBY_GENERAL:
		rp = "ID_LOBBY_GENERAL";
		break;
	case ID_RPC_REMOTE_ERROR:
		rp = "ID_RPC_REMOTE_ERROR";
		break;
	case ID_RPC_PLUGIN:
		rp = "ID_RPC_PLUGIN";
		break;
	case ID_FILE_LIST_REFERENCE_PUSH:
		rp = "ID_FILE_LIST_REFERENCE_PUSH";
		break;
	case ID_READY_EVENT_FORCE_ALL_SET:
		rp = "ID_READY_EVENT_FORCE_ALL_SET";
		break;
	case ID_ROOMS_EXECUTE_FUNC:
		rp = "ID_ROOMS_EXECUTE_FUNC";
		break;
	case ID_ROOMS_LOGON_STATUS:
		rp = "ID_ROOMS_LOGON_STATUS";
		break;
	case ID_ROOMS_HANDLE_CHANGE:
		rp = "ID_ROOMS_HANDLE_CHANGE";
		break;
	case ID_LOBBY2_SEND_MESSAGE:
		rp = "ID_LOBBY2_SEND_MESSAGE";
		break;
	case ID_LOBBY2_SERVER_ERROR:
		rp = "ID_LOBBY2_SERVER_ERROR";
		break;
	case ID_FCM2_NEW_HOST:
		rp = "ID_FCM2_NEW_HOST";
		break;
	case ID_FCM2_REQUEST_FCMGUID:
		rp = "ID_FCM2_REQUEST_FCMGUID";
		break;
	case ID_FCM2_RESPOND_CONNECTION_COUNT:
		rp = "ID_FCM2_RESPOND_CONNECTION_COUNT";
		break;
	case ID_FCM2_INFORM_FCMGUID:
		rp = "ID_FCM2_INFORM_FCMGUID";
		break;
	case ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT:
		rp = "ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT";
		break;
	case ID_FCM2_VERIFIED_JOIN_START:
		rp = "ID_FCM2_VERIFIED_JOIN_START";
		break;
	case ID_FCM2_VERIFIED_JOIN_CAPABLE:
		rp = "ID_FCM2_VERIFIED_JOIN_CAPABLE";
		break;
	case ID_FCM2_VERIFIED_JOIN_FAILED:
		rp = "ID_FCM2_VERIFIED_JOIN_FAILED";
		break;
	case ID_FCM2_VERIFIED_JOIN_ACCEPTED:
		rp = "ID_FCM2_VERIFIED_JOIN_ACCEPTED";
		break;
	case ID_FCM2_VERIFIED_JOIN_REJECTED:
		rp = "ID_FCM2_VERIFIED_JOIN_REJECTED";
		break;
	case ID_UDP_PROXY_GENERAL:
		rp = "ID_UDP_PROXY_GENERAL";
		break;
	case ID_SQLite3_EXEC:
		rp = "ID_SQLite3_EXEC";
		break;
	case ID_SQLite3_UNKNOWN_DB:
		rp = "ID_SQLite3_UNKNOWN_DB";
		break;
	case ID_SQLLITE_LOGGER:
		rp = "ID_SQLLITE_LOGGER";
		break;
	case ID_NAT_TYPE_DETECTION_REQUEST:
		rp = "ID_NAT_TYPE_DETECTION_REQUEST";
		break;
	case ID_NAT_TYPE_DETECTION_RESULT:
		rp = "ID_NAT_TYPE_DETECTION_RESULT";
		break;
	case ID_ROUTER_2_INTERNAL:
		rp = "ID_ROUTER_2_INTERNAL";
		break;
	case ID_ROUTER_2_FORWARDING_NO_PATH:
		rp = "ID_ROUTER_2_FORWARDING_NO_PATH";
		break;
	case ID_ROUTER_2_FORWARDING_ESTABLISHED:
		rp = "ID_ROUTER_2_FORWARDING_ESTABLISHED";
		break;
	case ID_ROUTER_2_REROUTED:
		rp = "ID_ROUTER_2_REROUTED";
		break;
	case ID_TEAM_BALANCER_INTERNAL:
		rp = "ID_TEAM_BALANCER_INTERNAL";
		break;
	case ID_TEAM_BALANCER_REQUESTED_TEAM_FULL:
		rp = "ID_TEAM_BALANCER_REQUESTED_TEAM_FULL";
		break;
	case ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED:
		rp = "ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED";
		break;
	case ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED:
		rp = "ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED";
		break;
	case ID_TEAM_BALANCER_TEAM_ASSIGNED:
		rp = "ID_TEAM_BALANCER_TEAM_ASSIGNED";
		break;
	case ID_LIGHTSPEED_INTEGRATION:
		rp = "ID_LIGHTSPEED_INTEGRATION";
		break;
	case ID_XBOX_LOBBY:
		rp = "ID_XBOX_LOBBY";
		break;
	case ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS:
		rp = "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS";
		break;
	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS:
		rp = "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS";
		break;
	case ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE:
		rp = "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE";
		break;
	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE:
		rp = "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE";
		break;
	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT:
		rp = "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT";
		break;
	case ID_TWO_WAY_AUTHENTICATION_NEGOTIATION:
		rp = "ID_TWO_WAY_AUTHENTICATION_NEGOTIATION";
		break;
	case ID_CLOUD_POST_REQUEST:
		rp = "ID_CLOUD_POST_REQUEST";
		break;
	case ID_CLOUD_RELEASE_REQUEST:
		rp = "ID_CLOUD_RELEASE_REQUEST";
		break;
	case ID_CLOUD_GET_REQUEST:
		rp = "ID_CLOUD_GET_REQUEST";
		break;
	case ID_CLOUD_GET_RESPONSE:
		rp = "ID_CLOUD_GET_RESPONSE";
		break;
	case ID_CLOUD_UNSUBSCRIBE_REQUEST:
		rp = "ID_CLOUD_UNSUBSCRIBE_REQUEST";
		break;
	case ID_CLOUD_SERVER_TO_SERVER_COMMAND:
		rp = "ID_CLOUD_SERVER_TO_SERVER_COMMAND";
		break;
	case ID_CLOUD_SUBSCRIPTION_NOTIFICATION:
		rp = "ID_CLOUD_SUBSCRIPTION_NOTIFICATION";
		break;
	case ID_LIB_VOICE:
		rp = "ID_LIB_VOICE";
		break;
	case ID_RELAY_PLUGIN:
		rp = "ID_RELAY_PLUGIN";
		break;
	case ID_NAT_REQUEST_BOUND_ADDRESSES:
		rp = "ID_NAT_REQUEST_BOUND_ADDRESSES";
		break;
	case ID_NAT_RESPOND_BOUND_ADDRESSES:
		rp = "ID_NAT_RESPOND_BOUND_ADDRESSES";
		break;
	case ID_FCM2_UPDATE_USER_CONTEXT:
		rp = "ID_FCM2_UPDATE_USER_CONTEXT";
		break;
	case ID_RESERVED_3:
		rp = "ID_RESERVED_3";
		break;
	case ID_RESERVED_4:
		rp = "ID_RESERVED_4";
		break;
	case ID_RESERVED_5:
		rp = "ID_RESERVED_5";
		break;
	case ID_RESERVED_6:
		rp = "ID_RESERVED_6";
		break;
	case ID_RESERVED_7:
		rp = "ID_RESERVED_7";
		break;
	case ID_RESERVED_8:
		rp = "ID_RESERVED_8";
		break;
	case ID_RESERVED_9:
		rp = "ID_RESERVED_9";
		break;
	case ID_USER_PACKET_ENUM:
		rp = "ID_USER_PACKET_ENUM";
		break;
	default:
		rp = "default packet type";
		break;
	}

	return rp;
}



int main()
{
	std::vector<char*> ipList = {"localhost","10.10.22.20", "122.99.85.34"};


	// check for client
	std::cout << "client (y/n) ";
	char cs;
	std::cin >> cs;
	//






	RakNet::RakPeerInterface * peer = RakNet::RakPeerInterface::GetInstance();

	if (tolower(cs) == 'y') // client
	{
		std::string ip;
		std::cout << "select ip to connect to\n\n";
		std::cout << "[00] - custom\n\n";
		for (size_t i = 0; i < ipList.size(); i++)
		{
			std::cout << "[" << i + 1 << "] - " << ipList[i] << "\n";
		}
		std::cout << " input number:";
		int selection = -1;
		std::cin >> selection;
		while (selection < 0 || selection > ipList.size())
		{
			std::cout << "invalid selection\ninput number:";
			std::cin >> selection;
		}
		if (selection == 0)
		{
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

			std::string packetType = getPacketType(*packet);
			std::cout << packetType << "\n";
		}
		else
		{
			std::cout << "packet was nullptr\n";
		}

		std::string message("Hello World!");

		peer->Send(message.c_str(), message.length()+1, HIGH_PRIORITY, RELIABLE, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
		
		peer->Shutdown(10);
		
	}
	else // server
	{
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
					std::string packetType = getPacketType(*packet);
					std::cout << packetType << "\n";

					// print packet data
					if (packet->length) // has data (first byte is for type enum)
					{
						std::string packetData = reinterpret_cast<char*>(packet->data);
						std::cout << "data[" << packet->length - 1 << "]:" << packetData << "\n";
						std::cout << "byteData:";
						for (int i = 0; i < packet->length; i++)
						{
							std::cout << (int)packet->data[i] << " ";
						}
						std::cout << "\b\n";
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


	system("pause");
	return 0;
}