#pragma once

#include <iostream>
#include <winsock2.h>
#include <windows.h>



#define WM_Conex (WM_USER + 1)
#define WM_msg (WM_USER + 2)

class Server {
public:
    Server();
    ~Server();
    void Start();
    static DWORD WINAPI HandleClient(LPVOID lpParam);
    static DWORD WINAPI ProgramMain(LPVOID lpParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static ATOM MyRegisterClass(HINSTANCE hInstance);
private:
    static const int PORT = 8080;
    static const int BUFFER_SIZE = 1024;
    HWND hWnd;
};
