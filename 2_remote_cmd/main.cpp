#include "server.hpp"
#include "client.hpp"
#include "cmd_executor.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage:\n"
            << "  " << argv[0] << " server <port>\n"
            << "  " << argv[0] << " client <address> <port> <command>\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "server") {
        if (argc < 3) {
            std::cerr << "Please provide port for server\n";
            return 1;
        }
        int port = std::stoi(argv[2]);
        Server server("127.0.0.1", port, std::make_unique<CommandExecutor>());
        server.start();

    } else if (mode == "client") {
        if (argc < 5) {
            std::cerr << "Please provide address, port and command\n";
            return 1;
        }
        std::string address = argv[2];
        int port = std::stoi(argv[3]);

        std::string command;
        for (int i = 4; i < argc; ++i) {
            if (i > 4) command += " ";
            command += argv[i];
        }

        try {
            Client client;
            client.connect(address, port);
            std::string output = client.send_command(command);
            std::cout << output;
        } catch (const std::exception& e) {
            std::cerr << "Client error: " << e.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Unknown mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}
