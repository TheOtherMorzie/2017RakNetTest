#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <exception>
#include <cstring>

// RakNet
#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <NatPunchthroughClient.h>

// fmod
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include "fmodExamples\user_created_sound.h"

#include "ExceptionC.h"

#include "Client.h"
#include "Server.h"
#include "WavFile.h"

#include "TTestBufferFiller.h"
#include "SoundPlayer.h"



#define SERVER_PORT 25505
#define CLIENT_PORT 25506
#define IP "122.99.85.34"


//std::string getPacketType(const RakNet::Packet & packet)
//{
//	std::string rp = "[blank]";
//
//	switch (packet.data[0])
//	{
//	case ID_CONNECTED_PING:
//		rp = "connected ping";
//		break;
//	case ID_UNCONNECTED_PING:
//		rp = "unconnected ping";
//		break;
//	case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
//		rp = "unconnected ping open connections";
//		break;
//	case ID_CONNECTED_PONG:
//		rp = "connected pong";
//		break;
//	case ID_DETECT_LOST_CONNECTIONS:
//		rp = "detect lost connections";
//		break;
//	case ID_OPEN_CONNECTION_REQUEST_1:
//		rp = "open connection request 1";
//		break;
//	case ID_OPEN_CONNECTION_REPLY_1:
//		rp = "open connection reply 1";
//		break;
//	case ID_OPEN_CONNECTION_REQUEST_2:
//		rp = "open connection request 2";
//		break;
//	case ID_OPEN_CONNECTION_REPLY_2:
//		rp = "open connection reply 2";
//		break;
//	case ID_CONNECTION_REQUEST:
//		rp = "connection request recieved";
//		break;
//	case ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY:
//		rp = "remote system requires public key";
//		break;
//	case ID_OUR_SYSTEM_REQUIRES_SECURITY:
//		rp = "our system requires security";
//		break;
//	case ID_PUBLIC_KEY_MISMATCH:
//		rp = "public key mismatch";
//		break;
//	case ID_OUT_OF_BAND_INTERNAL:
//		rp = "out of band internal";
//		break;
//	case ID_SND_RECEIPT_ACKED:
//		rp = "SND receipt acked";
//		break;
//	case ID_SND_RECEIPT_LOSS:
//		rp = "SND recipt loss";
//		break;
//	case ID_CONNECTION_REQUEST_ACCEPTED:
//		rp = "connection sucessful";
//		break;
//	case ID_CONNECTION_ATTEMPT_FAILED:
//		rp = "failed to connect";
//		break;
//	case ID_ALREADY_CONNECTED:
//		rp = "already connected";
//		break;
//	case ID_NEW_INCOMING_CONNECTION:
//		rp = "new incomming connection";
//		break;
//	case ID_NO_FREE_INCOMING_CONNECTIONS:
//		rp = "no free incomming connections";
//		break;
//	case ID_DISCONNECTION_NOTIFICATION:
//		rp = "disconnected";
//		break;
//	case ID_CONNECTION_LOST:
//		rp = "connection lost";
//		break;
//	case ID_CONNECTION_BANNED:
//		rp = "connection banned";
//		break;
//	case ID_INVALID_PASSWORD:
//		rp = "invalid password";
//		break;
//	case ID_INCOMPATIBLE_PROTOCOL_VERSION:
//		rp = "ID_INCOMPATIBLE_PROTOCOL_VERSION";
//		break;
//	case ID_IP_RECENTLY_CONNECTED:
//		rp = "ID_IP_RECENTLY_CONNECTED";
//		break;
//	case ID_TIMESTAMP:
//		rp = "timestamp";
//		break;
//	case ID_UNCONNECTED_PONG:
//		rp = "ID_UNCONNECTED_PONG";
//		break;
//	case ID_ADVERTISE_SYSTEM:
//		rp = "ID_ADVERTISE_SYSTEM";
//		break;
//	case ID_DOWNLOAD_PROGRESS:
//		rp = "download progress";
//		break;
//	case ID_REMOTE_DISCONNECTION_NOTIFICATION:
//		rp = "remote disconnet";
//		break;
//	case ID_REMOTE_CONNECTION_LOST:
//		rp = "remote connection lost";
//		break;
//	case ID_REMOTE_NEW_INCOMING_CONNECTION:
//		rp = "remote new incomming connection";
//		break;
//	case ID_FILE_LIST_TRANSFER_HEADER:
//		rp = "ID_FILE_LIST_TRANSFER_HEADER";
//		break;
//	case ID_FILE_LIST_TRANSFER_FILE:
//		rp = "ID_FILE_LIST_TRANSFER_FILE";
//		break;
//	case ID_FILE_LIST_REFERENCE_PUSH_ACK:
//		rp = "ID_FILE_LIST_REFERENCE_PUSH_ACK";
//		break;
//	case ID_DDT_DOWNLOAD_REQUEST:
//		rp = "ddt download request";
//		break;
//	case ID_TRANSPORT_STRING:
//		rp = "ID_TRANSPORT_STRING";
//		break;
//	case ID_REPLICA_MANAGER_CONSTRUCTION:
//		rp = "ID_REPLICA_MANAGER_CONSTRUCTION";
//		break;
//	case ID_REPLICA_MANAGER_SCOPE_CHANGE:
//		rp = "ID_REPLICA_MANAGER_SCOPE_CHANGE";
//		break;
//	case ID_REPLICA_MANAGER_SERIALIZE:
//		rp = "ID_REPLICA_MANAGER_SERIALIZE";
//		break;
//	case ID_REPLICA_MANAGER_DOWNLOAD_STARTED:
//		rp = "ID_REPLICA_MANAGER_DOWNLOAD_STARTED";
//		break;
//	case ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE:
//		rp = "ID_REPLICA_MANAGER_DOWNLOAD_COMPLETE";
//		break;
//	case ID_RAKVOICE_OPEN_CHANNEL_REQUEST:
//		rp = "ID_RAKVOICE_OPEN_CHANNEL_REQUEST";
//		break;
//	case ID_RAKVOICE_OPEN_CHANNEL_REPLY:
//		rp = "ID_RAKVOICE_OPEN_CHANNEL_REPLY";
//		break;
//	case ID_RAKVOICE_CLOSE_CHANNEL:
//		rp = "ID_RAKVOICE_CLOSE_CHANNEL";
//		break;
//	case ID_RAKVOICE_DATA: // ##################
//		rp = "ID_RAKVOICE_DATA";
//		break;
//	case ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE:
//		rp = "ID_AUTOPATCHER_GET_CHANGELIST_SINCE_DATE";
//		break;
//	case ID_AUTOPATCHER_CREATION_LIST:
//		rp = "ID_AUTOPATCHER_CREATION_LIST";
//		break;
//	case ID_AUTOPATCHER_DELETION_LIST:
//		rp = "ID_AUTOPATCHER_DELETION_LIST";
//		break;
//	case ID_AUTOPATCHER_GET_PATCH:
//		rp = "ID_AUTOPATCHER_GET_PATCH";
//		break;
//	case ID_AUTOPATCHER_PATCH_LIST:
//		rp = "ID_AUTOPATCHER_PATCH_LIST";
//		break;
//	case ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR:
//		rp = "ID_AUTOPATCHER_REPOSITORY_FATAL_ERROR";
//		break;
//	case ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES:
//		rp = "ID_AUTOPATCHER_CANNOT_DOWNLOAD_ORIGINAL_UNMODIFIED_FILES";
//		break;
//	case ID_AUTOPATCHER_FINISHED_INTERNAL:
//		rp = "ID_AUTOPATCHER_FINISHED_INTERNAL";
//		break;
//	case ID_AUTOPATCHER_FINISHED:
//		rp = "ID_AUTOPATCHER_FINISHED";
//		break;
//	case ID_AUTOPATCHER_RESTART_APPLICATION:
//		rp = "ID_AUTOPATCHER_RESTART_APPLICATION";
//		break;
//	case ID_NAT_PUNCHTHROUGH_REQUEST:
//		rp = "NAT punchthrough request";
//		break;
//	case ID_NAT_CONNECT_AT_TIME:
//		rp = "ID_NAT_CONNECT_AT_TIME";
//		break;
//	case ID_NAT_GET_MOST_RECENT_PORT:
//		rp = "ID_NAT_GET_MOST_RECENT_PORT";
//		break;
//	case ID_NAT_CLIENT_READY:
//		rp = "ID_NAT_CLIENT_READY";
//		break;
//	case ID_NAT_TARGET_NOT_CONNECTED:
//		rp = "ID_NAT_TARGET_NOT_CONNECTED";
//		break;
//	case ID_NAT_TARGET_UNRESPONSIVE:
//		rp = "ID_NAT_TARGET_UNRESPONSIVE";
//		break;
//	case ID_NAT_CONNECTION_TO_TARGET_LOST:
//		rp = "ID_NAT_CONNECTION_TO_TARGET_LOST";
//		break;
//	case ID_NAT_ALREADY_IN_PROGRESS:
//		rp = "ID_NAT_ALREADY_IN_PROGRESS";
//		break;
//	case ID_NAT_PUNCHTHROUGH_FAILED:
//		rp = "NAT punchthrough failed";
//		break;
//	case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
//		rp = "NAT punchthrough succeeded";
//		break;
//	case ID_READY_EVENT_SET:
//		rp = "ID_READY_EVENT_SET";
//		break;
//	case ID_READY_EVENT_UNSET:
//		rp = "ID_READY_EVENT_UNSET";
//		break;
//	case ID_READY_EVENT_ALL_SET:
//		rp = "ID_READY_EVENT_ALL_SET";
//		break;
//	case ID_READY_EVENT_QUERY:
//		rp = "ID_READY_EVENT_QUERY";
//		break;
//	case ID_LOBBY_GENERAL:
//		rp = "ID_LOBBY_GENERAL";
//		break;
//	case ID_RPC_REMOTE_ERROR:
//		rp = "ID_RPC_REMOTE_ERROR";
//		break;
//	case ID_RPC_PLUGIN:
//		rp = "ID_RPC_PLUGIN";
//		break;
//	case ID_FILE_LIST_REFERENCE_PUSH:
//		rp = "ID_FILE_LIST_REFERENCE_PUSH";
//		break;
//	case ID_READY_EVENT_FORCE_ALL_SET:
//		rp = "ID_READY_EVENT_FORCE_ALL_SET";
//		break;
//	case ID_ROOMS_EXECUTE_FUNC:
//		rp = "ID_ROOMS_EXECUTE_FUNC";
//		break;
//	case ID_ROOMS_LOGON_STATUS:
//		rp = "ID_ROOMS_LOGON_STATUS";
//		break;
//	case ID_ROOMS_HANDLE_CHANGE:
//		rp = "ID_ROOMS_HANDLE_CHANGE";
//		break;
//	case ID_LOBBY2_SEND_MESSAGE:
//		rp = "ID_LOBBY2_SEND_MESSAGE";
//		break;
//	case ID_LOBBY2_SERVER_ERROR:
//		rp = "ID_LOBBY2_SERVER_ERROR";
//		break;
//	case ID_FCM2_NEW_HOST:
//		rp = "ID_FCM2_NEW_HOST";
//		break;
//	case ID_FCM2_REQUEST_FCMGUID:
//		rp = "ID_FCM2_REQUEST_FCMGUID";
//		break;
//	case ID_FCM2_RESPOND_CONNECTION_COUNT:
//		rp = "ID_FCM2_RESPOND_CONNECTION_COUNT";
//		break;
//	case ID_FCM2_INFORM_FCMGUID:
//		rp = "ID_FCM2_INFORM_FCMGUID";
//		break;
//	case ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT:
//		rp = "ID_FCM2_UPDATE_MIN_TOTAL_CONNECTION_COUNT";
//		break;
//	case ID_FCM2_VERIFIED_JOIN_START:
//		rp = "ID_FCM2_VERIFIED_JOIN_START";
//		break;
//	case ID_FCM2_VERIFIED_JOIN_CAPABLE:
//		rp = "ID_FCM2_VERIFIED_JOIN_CAPABLE";
//		break;
//	case ID_FCM2_VERIFIED_JOIN_FAILED:
//		rp = "ID_FCM2_VERIFIED_JOIN_FAILED";
//		break;
//	case ID_FCM2_VERIFIED_JOIN_ACCEPTED:
//		rp = "ID_FCM2_VERIFIED_JOIN_ACCEPTED";
//		break;
//	case ID_FCM2_VERIFIED_JOIN_REJECTED:
//		rp = "ID_FCM2_VERIFIED_JOIN_REJECTED";
//		break;
//	case ID_UDP_PROXY_GENERAL:
//		rp = "ID_UDP_PROXY_GENERAL";
//		break;
//	case ID_SQLite3_EXEC:
//		rp = "ID_SQLite3_EXEC";
//		break;
//	case ID_SQLite3_UNKNOWN_DB:
//		rp = "ID_SQLite3_UNKNOWN_DB";
//		break;
//	case ID_SQLLITE_LOGGER:
//		rp = "ID_SQLLITE_LOGGER";
//		break;
//	case ID_NAT_TYPE_DETECTION_REQUEST:
//		rp = "ID_NAT_TYPE_DETECTION_REQUEST";
//		break;
//	case ID_NAT_TYPE_DETECTION_RESULT:
//		rp = "ID_NAT_TYPE_DETECTION_RESULT";
//		break;
//	case ID_ROUTER_2_INTERNAL:
//		rp = "ID_ROUTER_2_INTERNAL";
//		break;
//	case ID_ROUTER_2_FORWARDING_NO_PATH:
//		rp = "ID_ROUTER_2_FORWARDING_NO_PATH";
//		break;
//	case ID_ROUTER_2_FORWARDING_ESTABLISHED:
//		rp = "ID_ROUTER_2_FORWARDING_ESTABLISHED";
//		break;
//	case ID_ROUTER_2_REROUTED:
//		rp = "ID_ROUTER_2_REROUTED";
//		break;
//	case ID_TEAM_BALANCER_INTERNAL:
//		rp = "ID_TEAM_BALANCER_INTERNAL";
//		break;
//	case ID_TEAM_BALANCER_REQUESTED_TEAM_FULL:
//		rp = "ID_TEAM_BALANCER_REQUESTED_TEAM_FULL";
//		break;
//	case ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED:
//		rp = "ID_TEAM_BALANCER_REQUESTED_TEAM_LOCKED";
//		break;
//	case ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED:
//		rp = "ID_TEAM_BALANCER_TEAM_REQUESTED_CANCELLED";
//		break;
//	case ID_TEAM_BALANCER_TEAM_ASSIGNED:
//		rp = "ID_TEAM_BALANCER_TEAM_ASSIGNED";
//		break;
//	case ID_LIGHTSPEED_INTEGRATION:
//		rp = "ID_LIGHTSPEED_INTEGRATION";
//		break;
//	case ID_XBOX_LOBBY:
//		rp = "ID_XBOX_LOBBY";
//		break;
//	case ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS:
//		rp = "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_SUCCESS";
//		break;
//	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS:
//		rp = "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_SUCCESS";
//		break;
//	case ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE:
//		rp = "ID_TWO_WAY_AUTHENTICATION_INCOMING_CHALLENGE_FAILURE";
//		break;
//	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE:
//		rp = "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_FAILURE";
//		break;
//	case ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT:
//		rp = "ID_TWO_WAY_AUTHENTICATION_OUTGOING_CHALLENGE_TIMEOUT";
//		break;
//	case ID_TWO_WAY_AUTHENTICATION_NEGOTIATION:
//		rp = "ID_TWO_WAY_AUTHENTICATION_NEGOTIATION";
//		break;
//	case ID_CLOUD_POST_REQUEST:
//		rp = "ID_CLOUD_POST_REQUEST";
//		break;
//	case ID_CLOUD_RELEASE_REQUEST:
//		rp = "ID_CLOUD_RELEASE_REQUEST";
//		break;
//	case ID_CLOUD_GET_REQUEST:
//		rp = "ID_CLOUD_GET_REQUEST";
//		break;
//	case ID_CLOUD_GET_RESPONSE:
//		rp = "ID_CLOUD_GET_RESPONSE";
//		break;
//	case ID_CLOUD_UNSUBSCRIBE_REQUEST:
//		rp = "ID_CLOUD_UNSUBSCRIBE_REQUEST";
//		break;
//	case ID_CLOUD_SERVER_TO_SERVER_COMMAND:
//		rp = "ID_CLOUD_SERVER_TO_SERVER_COMMAND";
//		break;
//	case ID_CLOUD_SUBSCRIPTION_NOTIFICATION:
//		rp = "ID_CLOUD_SUBSCRIPTION_NOTIFICATION";
//		break;
//	case ID_LIB_VOICE:
//		rp = "ID_LIB_VOICE";
//		break;
//	case ID_RELAY_PLUGIN:
//		rp = "ID_RELAY_PLUGIN";
//		break;
//	case ID_NAT_REQUEST_BOUND_ADDRESSES:
//		rp = "ID_NAT_REQUEST_BOUND_ADDRESSES";
//		break;
//	case ID_NAT_RESPOND_BOUND_ADDRESSES:
//		rp = "ID_NAT_RESPOND_BOUND_ADDRESSES";
//		break;
//	case ID_FCM2_UPDATE_USER_CONTEXT:
//		rp = "ID_FCM2_UPDATE_USER_CONTEXT";
//		break;
//	case ID_RESERVED_3:
//		rp = "ID_RESERVED_3";
//		break;
//	case ID_RESERVED_4:
//		rp = "ID_RESERVED_4";
//		break;
//	case ID_RESERVED_5:
//		rp = "ID_RESERVED_5";
//		break;
//	case ID_RESERVED_6:
//		rp = "ID_RESERVED_6";
//		break;
//	case ID_RESERVED_7:
//		rp = "ID_RESERVED_7";
//		break;
//	case ID_RESERVED_8:
//		rp = "ID_RESERVED_8";
//		break;
//	case ID_RESERVED_9:
//		rp = "ID_RESERVED_9";
//		break;
//	case ID_USER_PACKET_ENUM:
//		rp = "ID_USER_PACKET_ENUM";
//		break;
//	default:
//		rp = "default packet type";
//		break;
//	}
//
//	return rp;
//}


int main()
{
	try
	{
		if (true){
			WavFile wf;
			int result = wf.loadSoundFile("../sounds/piano2.wav");

			switch (result)
			{
			case WavFile::WFEC_NO_ERROR:
				printf("#########wav file loaded properly\n");
				break;
			case WavFile::WFEC_FILE_DOES_NOT_EXIST:
				printf("wav file does not exist\n");
				break;
			case WavFile::WFEC_FILE_FAILED_TO_OPEN:
				printf("wav file failed to open\n");
				break;
			case WavFile::WFEC_FILE_NOT_WAV_TYPE:
				printf("file is not a wav file\n");
				break;
			case WavFile::WFEC_FILE_OVERRUN:
				printf("wav file overran\n");
				break;
			default:
				printf("wav file failed");
				break;
			}

			TTestBufferFiller * ttbf = new TTestBufferFiller();
			ttbf->addFile(&wf);

			SoundPlayer * sp = new SoundPlayer();
			sp->init(*ttbf);

			int ttbfError;
			std::mutex ttbfMutex;
			TTestBufferFiller ** ttbfp = &ttbf;
			std::thread * ttbfThread = new std::thread(TTestBufferFiller::thread, &ttbfError, ttbfp, &ttbfMutex);

			bool run = true;
			while (run)
			{
				ttbfMutex.lock();
				run = ttbf->isActive();
				ttbfMutex.unlock();

				// check if soundPlayer is playing
				if (sp->getPause())
				{
					printf("######### SoundPlayer playstate: true\n");
				}
				else
				{
					printf("######### SoundPlayer playstate: false\n");
				}

				FMOD::Sound * s = sp->getSound();
				FMOD::Channel * c = sp->getChannel();
				FMOD::System * sy = sp->getSystem();

				unsigned int pos;
				FMOD_RESULT fResult = c->getPosition(&pos, FMOD_TIMEUNIT_MS);
				if (fResult != FMOD_OK)
				{
					std::string f;
					f += fResult;
					f += " - ";
					f += FMOD_ErrorString(fResult);
					char * c = new char[f.size()];
					strcpy(c, f.c_str());
					THROWC(c);
				}

				printf("######### SoundPlayer Playback position(ms): %d\n", pos);

				// avoid locking up the thread
				std::chrono::milliseconds timespan(100);
				std::this_thread::sleep_for(timespan);
			}

			printf("#########bufferFillerDone!\n");

			system("pause");

		}

		system("pause");

		FMODExamples::FMOD_MainUCS();

		system("pause");



		std::vector<char*> ipList = { "localhost","10.10.22.20", "122.99.85.34" };


		// check for client
		std::cout << "client (y/n) ";
		char cs;
		std::cin >> cs;
		//


		std::thread * networkerThread = nullptr;

		if (tolower(cs) == 'y') // client
		{
			int * returnVal = new int(0);
			//int * returnVal;
			Client * interfaceClass = nullptr;
			std::mutex interfaceMutex;
			networkerThread = new std::thread(Client::thread, returnVal, &interfaceClass, &interfaceMutex);

			bool threadSetup = false;
			while (threadSetup == false)
			{
				interfaceMutex.lock();
				if (interfaceClass != nullptr) // thread setup class correctly
				{
					threadSetup = true;
					printf("client thread class is active!\n");
				}
				else if (*returnVal != 0)
				{
					interfaceMutex.unlock();
					threadSetup = true;
					THROWC(strcat("thread returned error code ", (char*)(*returnVal)));
				}
				interfaceMutex.unlock();
			}
			if (!threadSetup)
			{
				THROWC("thread failed to setup");
			}

			printf("send messages to server:\n");

			while (true)
			{
				std::string message;
				std::cin.clear();
				std::cin.ignore(99999999999999, '\n');
				std::cin >> message;
				if (message.length() > 0)
				{
					std::cout << "sending message \"" << message << "\"\n";
					interfaceClass->sendMessage(message);
				}
				else
				{
					printf("message is empty\n");
				}
				printf("\n");
			}
		}
		else // server
		{

			printf("loading wav File\n");

			WavFile wf;
			int result = wf.loadSoundFile("../sounds/piano2.wav");

			switch (result)
			{
			case WavFile::WFEC_NO_ERROR:
				printf("wav file loaded properly\n");
				break;
			case WavFile::WFEC_FILE_DOES_NOT_EXIST:
				printf("wav file does not exist\n");
				break;
			case WavFile::WFEC_FILE_FAILED_TO_OPEN:
				printf("wav file failed to open\n");
				break;
			case WavFile::WFEC_FILE_NOT_WAV_TYPE:
				printf("file is not a wav file\n");
				break;
			case WavFile::WFEC_FILE_OVERRUN:
				printf("wav file overran\n");
				break;
			default:
				printf("wav file failed");
				break;
			}

			FMOD_RESULT fResult;
			FMOD::System * fSystem;
			FMOD_CREATESOUNDEXINFO soundInfo;
			FMOD::Sound * fSound;
			FMOD::Channel * fChannel;

			std::vector<WavFile::subChunk> wfsc = wf.getDataSubchunkIndexList();

			WavFile::subChunk * wffmtIndex = nullptr;
			WavFile::subChunk * wfdataindex = nullptr;

			for (size_t i = 0; i < wfsc.size(); i++)
			{
				char idc[5];
				strcpy(idc, wfsc[i].ID);
				idc[4] = '\0';
				std::string id(idc);
				std::string fmt("fmt ");
				std::string data("data");
				if (id == std::string("fmt "))
				{
					if (wffmtIndex != nullptr)
					{
						delete wffmtIndex;
					}
					wffmtIndex = new WavFile::subChunk;
					(*wffmtIndex) = wfsc[i];
				}
				if (id == std::string("data"))
				{
					if (wfdataindex != nullptr)
					{
						delete wfdataindex;
					}
					wfdataindex = new WavFile::subChunk;
					(*wfdataindex) = wfsc[i];
				}
			}

			unsigned __int16 audioFormat;
			memcpy(&audioFormat, &(wf.getRawData()[(wffmtIndex->index + 8)]), 2);

			unsigned __int16 channlCount;
			memcpy(&channlCount, &(wf.getRawData()[(wffmtIndex->index + 10)]), 2);

			unsigned __int32 sampleRate;
			memcpy(&sampleRate, &(wf.getRawData()[(wffmtIndex->index + 12)]), 4);

			unsigned __int16 bitsPerSample;
			memcpy(&bitsPerSample, &(wf.getRawData()[(wffmtIndex->index + 22)]), 2);

			//FMOD_SOUND_PCMREAD_CALLBACK * pcmreadcallback = new FMOD_SOUND_PCMREAD_CALLBACK(pcmreadcallback);

			soundInfo.cbsize = sizeof(soundInfo);
			soundInfo.length = wfdataindex->size;
			soundInfo.numchannels = channlCount;
			soundInfo.defaultfrequency = sampleRate; // I hope it means bitrate
			//soundInfo.pcmreadcallback = ;

			if (audioFormat == 1) // PCM format
			{
				switch (bitsPerSample)
				{
				case 8:
					soundInfo.format = FMOD_SOUND_FORMAT_PCM8;
					break;
				case 16:
					soundInfo.format = FMOD_SOUND_FORMAT_PCM16;
					break;
				case 24:
					soundInfo.format = FMOD_SOUND_FORMAT_PCM24;
					break;
				case 32:
					soundInfo.format = FMOD_SOUND_FORMAT_PCM32;
					break;
				default:
					THROWC("unknown wav bitsPerSample");
					break;
				}
			}
			else
			{
				THROWC("unknow wav file format");
			}


			

			

			// create system
			fResult = FMOD::System_Create(&fSystem);
			if (fResult != FMOD_OK)
			{
				std::string f;
				f += fResult;
				f += " - ";
				f += FMOD_ErrorString(fResult);
				char * c = new char[f.size()];
				strcpy(c, f.c_str());
				THROWC(c);
			}

			fResult = fSystem->init(512, FMOD_INIT_NORMAL, 0);
			if (fResult != FMOD_OK)
			{
				std::string f;
				f += fResult;
				f += " - ";
				f += FMOD_ErrorString(fResult);
				char * c = new char[f.size()];
				strcpy(c, f.c_str());
				THROWC(c);
			}

			// create sound
			fResult = fSystem->createSound(wf.getSoundData(), FMOD_OPENRAW, &soundInfo, &fSound);
			if (fResult != FMOD_OK)
			{
				std::string f;
				f += toascii(fResult);
				f += " - ";
				f += FMOD_ErrorString(fResult);
				char * c = new char[f.size()];
				strcpy(c, f.c_str());
				THROWC(c);
			}

			// play sound
			fResult = fSystem->playSound(fSound, 0, false, &fChannel);
			if (fResult != FMOD_OK)
			{
				std::string f;
				f += fResult;
				f += " - ";
				f += FMOD_ErrorString(fResult);
				char * c = new char[f.size()];
				strcpy(c, f.c_str());
				THROWC(c);
			}



			int * returnVal = new int(0);
			//int * returnVal;
			Server * interfaceClass = nullptr;
			std::mutex interfaceMutex;
			networkerThread = new std::thread(Server::thread, returnVal, &interfaceClass, &interfaceMutex);

			bool threadSetup = false;
			while (threadSetup == false)
			{
				interfaceMutex.lock();
				if (interfaceClass != nullptr) // thread setup class correctly
				{
					threadSetup = true;
					printf("Server thread class is active!\n");
				}
				else if (*returnVal != 0)
				{
					interfaceMutex.unlock();
					threadSetup = true;
					THROWC(strcat("thread returned error code ", (char*)(*returnVal)));
				}
				interfaceMutex.unlock();
			}
			if (!threadSetup)
			{
				THROWC("thread failed to setup");
			}
		}

		if (networkerThread != nullptr)
		{
			networkerThread->join();
			delete networkerThread;
			networkerThread = nullptr;
		}


		system("pause");
		return 0;
	}
	catch (const std::exception & e)
	{
		std::cout << "Standard exception: " << e.what() << "\n";
		system("pause");
		return 1;
	}
	catch (std::system_error & e)
	{
		std::cout << "System error (" << e.code().message() << ") " << e.what() << "\n";
		system("pause");
		return 1;
	}
	catch (const ExceptionC & e)
	{
		std::cout << "Error! [" << e.line << "] - " << e.error << "\n" << e.file << "\n";
		system("pause");
		return 1;
	}
	catch (...)
	{
		printf("general execption was throw\n");
		system("pause");
		return 1;
	}
}