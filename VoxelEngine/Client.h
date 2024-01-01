#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <iostream>

class Client
{
public:
	Client();
	~Client();

	void start();

private:
	SOCKET sock;
	std::thread thread;

	bool connect(const char* ip, int port);
	void communicateThread();
};