#include "Server.h"

#pragma comment(lib, "ws2_32.lib")

Server::Server() {
    HINSTANCE hInstance = GetModuleHandleA(0);
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

    RegisterClassExW(&wcex);

    hWnd = CreateWindowW(L"MyClass", L"", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock." << std::endl;
        throw std::runtime_error("WSAStartup failed");
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur lors de la création du socket." << std::endl;
        WSACleanup();
        throw std::runtime_error("Socket creation failed");
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la liaison du socket à l'adresse et au port." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        throw std::runtime_error("Socket binding failed");
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erreur lors de la mise en attente de connexions." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Le serveur écoute sur le port " << PORT << " ..." << std::endl;

    WSAAsyncSelect(serverSocket, hWnd, WM_Conex, FD_ACCEPT);
}

Server::~Server() {
    Stop();
}

void Server::Start() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Server::Stop() {
    closesocket(serverSocket);
    WSACleanup();
}

LRESULT CALLBACK Server::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_Conex:
        switch (WSAGETSELECTEVENT(lParam)) {
        case FD_ACCEPT:
            SOCKET clientSocket = accept(wParam, nullptr, nullptr);
            WSAAsyncSelect(clientSocket, hWnd, WM_msg, FD_READ | FD_CLOSE);
            std::cout << "Connexion Accepted: " << wParam << std::endl;
            break;
        }
        break;

    case WM_msg:
        switch (WSAGETSELECTEVENT(lParam)) {
        case FD_READ: {
            char buffer[BUFFER_SIZE];
            int count = recv(wParam, buffer, sizeof(buffer), 0);

            buffer[count] = '\0';
            if (count > 0) {
                std::cout << "Message du client: " << buffer << std::endl;
            }
            break;
        }
        case FD_CLOSE:
            std::cout << "Connexion Close: " << wParam << std::endl;
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
