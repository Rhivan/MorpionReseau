#pragma once

#include <string>
#include <winsock2.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class Client {
public:
	Client(const char* serverIp, int port);
	~Client();
	void run();
	void createMessage(std::string user, std::string player, int x, int y);
private:
	const char* SERVER_IP;
	int PORT;
	SOCKET clientSocket;
	HINSTANCE hInst;
	void connect();
};
