#include "Server.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>

int main() {
    try {
        Server server;
        server.Start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
