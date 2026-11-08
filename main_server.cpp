#include <iostream>
#include <string>
#include "Server.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <mail-spool-directoryname>" << std::endl;
        return 1;
    }

    int port = 0;
    try {
        port = std::stoi(argv[1]);
    } catch (...) {
        std::cerr << "Invalid port number" << std::endl;
        return 1;
    }

    std::string spoolDir = argv[2];

    Server server(port, spoolDir);
    server.start();

    return 0;
}