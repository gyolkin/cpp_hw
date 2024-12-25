#include "client.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }
    try {
        std::string username;
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        if (username.empty()) {
            username = "anonymous";
        }
        Client client(username);
        int port = std::stoi(argv[1]);
        client.start(port);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
