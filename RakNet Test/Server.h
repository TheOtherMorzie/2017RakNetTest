#pragma once
#include "ThreadBase.h"

#include <mutex>

class Server : ThreadBase
{
public:
	static void thread(int * errorCode, Server ** interfaceClass, std::mutex * interfaceMutex);

	Server();
	~Server();
};

