#include <iostream>
#include <winsock2.h>
#include <windows.h>

//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"

#pragma comment(lib, "ws2_32.lib")

const int PORT = 8080;
const int BUFFER_SIZE = 4096;

#define WM_Conex (WM_USER+1)
#define WM_msg (WM_USER+2)

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_Conex:
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
			int c = 0;
			SOCKET clientSocket = accept(wParam, nullptr, nullptr);
			WSAAsyncSelect(clientSocket, hWnd, WM_msg, FD_READ | FD_CLOSE);
			std::cout << "Connexion Accepted: " << wParam << std::endl;
			break;
		}
		break;

	case WM_msg:
	{
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
			char buffer[BUFFER_SIZE];
			
			int count = recv(wParam, buffer, sizeof(buffer), 0);

			buffer[count] = '\0';
			if (count > 0)
			{
				std::cout << "Message du client: " << buffer << std::endl;
			}
			//if ( count>=0)
			//buffer[count] = 0;
			//std::cout << "Message du client: " << buffer << std::endl;

			//int byteRead =  recv(wParam, buffer, sizeof(buffer), 0);
			//{
			//    buffer[byteRead] = '\0';
			//    std::cout << "Message du client : " << buffer << std::endl;

			//    // Réponse au client (ici, simplement renvoyer le même message)
			//    send(wParam, buffer, byteRead, 0);
			//}

			//int a = 0;
			//char buf[4];
			//recv(wParam, buf, 4, 0);
			//unsigned int size;
			//memcpy(&size, buf, 4);

			//char* msg = new char[size];
			//recv(wParam, msg, size, 0);
			//// json

			//delete[] msg;

			break;
		}
		case FD_CLOSE:
		{
			int b = 0;
			std::cout << "Connexion Close: " << wParam << std::endl;
			break;
		}
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MyClass";
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

int main() {
	HINSTANCE hInstance = GetModuleHandleA(0);
	MyRegisterClass(hInstance);
	HWND hWnd = CreateWindowW(L"MyClass", L"", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Erreur lors de l'initialisation de Winsock." << std::endl;
		return EXIT_FAILURE;
	}

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Erreur lors de la création du socket." << std::endl;
		WSACleanup();
		return EXIT_FAILURE;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(PORT);

	if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
		std::cerr << "Erreur lors de la liaison du socket à l'adresse et au port." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Erreur lors de la mise en attente de connexions." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}

	std::cout << "Le serveur écoute sur le port " << PORT << " ..." << std::endl;

	WSAAsyncSelect(serverSocket, hWnd, WM_Conex, FD_ACCEPT);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/*

	SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Erreur lors de l'acceptation de la connexion entrante." << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}

	std::cout << "Connexion acceptée." << std::endl;

	char buffer[BUFFER_SIZE];
	int bytesRead;
	while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
		buffer[bytesRead] = '\0';
		std::cout << "Message du client : " << buffer << std::endl;
		send(clientSocket, buffer, bytesRead, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	*/
	WSACleanup();

	// Attente d'une entrée pour maintenir la console ouverte
	std::cout << "Appuyez sur Enter pour quitter...";
	std::cin.get();

	return EXIT_SUCCESS;
}