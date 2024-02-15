#include "client.h"
#include <iostream>
#include <windows.h>

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
    createMessage("Guilherme", "O", 1, 1);
//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Analyse les sélections de menu :
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		TextOut(hdc, 10, 10, L"Hello, Windows!", 15);
		

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


