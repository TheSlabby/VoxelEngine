#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

void Client::start() {
	std::cout << "ClIENT::Attempting to connect..." << std::endl;
	if (connect("localhost", 4545) == false) {
		std::cout << "CLIENT::Error setting up socket" << std::endl;
		return;
	}

	//start listening thread
	thread = std::thread(&Client::communicateThread, this);

}

bool Client::connect(const char* ip, int port) {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
		return false;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ip, &hint.sin_addr);

	int connResult = ::connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		return false;
	}

	//success
	this->sock = sock;
	return true;
}

void Client::communicateThread() {
	char buf[4096];
	while (true) {
		ZeroMemory(buf, 4096);

		//wait for data
		int bytesIn = recv(sock, buf, 4096, 0);
		if (bytesIn > 0) {
			std::cout << "SERVER> " << std::string(buf, 0, bytesIn) << std::endl;
		}
	}
}

Client::Client() {

}
Client::~Client() {
	//cleanup
	closesocket(sock);

	if (thread.joinable()) {
		thread.join();
	}

	WSACleanup();
}
