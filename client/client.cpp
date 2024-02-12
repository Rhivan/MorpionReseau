#include "client.h"
#include <iostream>
#include <windows.h>
#include "framework.h"
#include "Resource.h"

#pragma comment(lib, "ws2_32.lib")

Client::Client(const char* serverIp, int port) : SERVER_IP(serverIp), PORT(port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock." << std::endl;
        exit(EXIT_FAILURE);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

Client::~Client() {
    closesocket(clientSocket);
    WSACleanup();
}

void Client::connect() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddress.sin_port = htons(PORT);

    if (::connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la connexion au serveur." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void Client::createMessage(std::string user, std::string player, int x, int y) {
    rapidjson::Document message;
    message.SetObject();

    rapidjson::Value Username;
    Username.SetString(user.c_str(), message.GetAllocator());
    message.AddMember("Username", Username, message.GetAllocator());

    rapidjson::Value PlayerToken;
    PlayerToken.SetString(player.c_str(), message.GetAllocator());
    message.AddMember("PlayerToken", PlayerToken, message.GetAllocator());

    rapidjson::Value X;
    X.SetInt(x);
    message.AddMember("x", X, message.GetAllocator());

    rapidjson::Value Y;
    Y.SetInt(y);
    message.AddMember("y", Y, message.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    message.Accept(writer);

    send(clientSocket, buffer.GetString(), buffer.GetLength(), 0);
}

void Client::run() {
    connect();
    createMessage("Guilherme", "X", 1, 1);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Client client("127.0.0.1", 8080);
    client.run();
    return 0;
}