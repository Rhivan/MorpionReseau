#include "Client.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Client client("127.0.0.1", 8080);
    client.run();
    return 0;
}
