#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <winsock2.h>
#include <windows.h>

const int BUFFER_SIZE = 4096;

class Server {
public:
    Server();
    ~Server();
    void Start();
    void Stop();

private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static const int PORT = 8080;
    HWND hWnd;
    SOCKET serverSocket;

    static const UINT WM_Conex = WM_USER + 1;
    static const UINT WM_msg = WM_USER + 2;
};

#endif // SERVER_H
