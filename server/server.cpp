#include "Server.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

Server::Server() {}

Server::~Server() {}

void Server::Start() {
    HINSTANCE hInstance = GetModuleHandleA(0);
    MyRegisterClass(hInstance);

    // Create a hidden window to handle messages
    HWND hWnd = CreateWindowW(L"MyClass", L"MyServerWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        std::cerr << "Erreur lors de la création de la fenêtre." << std::endl;
        return;
    }

    // Start the server in a separate thread
    HANDLE mainThreadHandle = CreateThread(nullptr, 0, ProgramMain, reinterpret_cast<LPVOID>(hWnd), 0, nullptr);
    if (mainThreadHandle == nullptr) {
        std::cerr << "Erreur lors de la création du thread principal." << std::endl;
        return;
    }
    else {
        std::cout << "Thread principal créé avec succès." << std::endl;
    }

    // Run the message loop to dispatch messages to WndProc
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


DWORD WINAPI Server::HandleClient(LPVOID lpParam) {
    SOCKET clientSocket = reinterpret_cast<SOCKET>(lpParam);

    char buffer[BUFFER_SIZE];
    int count;

    while ((count = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[count] = '\0';
        std::cout << buffer << " Viens de se connecter !" << std::endl;
    }

    closesocket(clientSocket);
    std::cout << "Connexion fermée avec un client." << std::endl;

    return 0;
}

DWORD WINAPI Server::ProgramMain(LPVOID lpParam) {
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

    WSAAsyncSelect(serverSocket, static_cast<HWND>(lpParam), WM_Conex, FD_ACCEPT);

    return 0;
}

LRESULT CALLBACK Server::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_Conex:
        switch (WSAGETSELECTEVENT(lParam)) {
        case FD_ACCEPT: {
            SOCKET clientSocket = accept(wParam, nullptr, nullptr);
            WSAAsyncSelect(clientSocket, hWnd, WM_msg, FD_READ | FD_CLOSE);
            std::cout << "Connexion Accepted" << wParam << std::endl;

            HANDLE threadHandle = CreateThread(nullptr, 0, HandleClient, reinterpret_cast<LPVOID>(clientSocket), 0, nullptr);
            if (threadHandle == NULL) {
                std::cerr << "Erreur lors de la création du thread pour gérer le client." << std::endl;
                closesocket(clientSocket);
            }
            else {
                std::cout << "Thread client créé avec succès." << std::endl;
            }
            break;
        }
        }
        break;

    case WM_msg: {
        switch (WSAGETSELECTEVENT(lParam)) {
        case FD_READ: {
            char buffer[BUFFER_SIZE];
            int count = recv(wParam, buffer, sizeof(buffer), 0);
            std::cout << "Message du client : " << buffer << std::endl;
            break;
        }
        case FD_CLOSE: {
            std::cout << "Connexion Close" << wParam << std::endl;
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

ATOM Server::MyRegisterClass(HINSTANCE hInstance) {
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
