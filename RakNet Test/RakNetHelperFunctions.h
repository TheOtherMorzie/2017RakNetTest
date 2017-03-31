#pragma once

#include <string>

#include <RakNetTypes.h>

namespace RakNetHelpers
{

	enum UserMessageID
	{

	};

	std::string getPacketType(const RakNet::Packet & packet);
}