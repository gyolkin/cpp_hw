#include "server.hpp"
#include <cstring>
#include <iostream>
#include <vector>
#include <thread>

Server::Server(const std::string& address, int port, std::unique_ptr<CommandExecutor> executor)
    : command_executor_(std::move(executor)), running_(false)
{
    listening_socket_.set_reuse_address(true);
    listening_socket_.bind(address, static_cast<uint16_t>(port));
    listening_socket_.listen();
}

Server::~Server() {
    stop();
}

void Server::start() {
    running_ = true;
    std::cout << "Server started on port " << listening_socket_.get_port() << std::endl;
    while (running_) {
        try {
            std::string client_addr;
            uint16_t client_port;
            TCPSocket client = listening_socket_.accept(client_addr, client_port);
            std::thread(&Server::handle_client, this, std::move(client)).detach();
        } catch (const std::exception& e) {
            if (running_) {
                std::cerr << "Accept error: " << e.what() << std::endl;
            }
        }
    }
}

void Server::stop() {
    running_ = false;
    listening_socket_.close();
}

void Server::handle_client(TCPSocket client_socket) {
    try {
        uint32_t cmd_len_net;
        size_t received = client_socket.receive(&cmd_len_net, sizeof(cmd_len_net));
        if (received < sizeof(cmd_len_net)) {
            return;
        }
        uint32_t cmd_len = ntohl(cmd_len_net);

        std::vector<char> cmd_buffer(cmd_len + 1, 0);
        received = 0;
        while (received < cmd_len) {
            size_t received_bytes = client_socket.receive(cmd_buffer.data() + received, cmd_len - received);
            if (received_bytes == 0) return;
            received += received_bytes;
        }

        std::string command = cmd_buffer.data();
        std::string output = command_executor_->execute(command);

        uint32_t output_size = static_cast<uint32_t>(output.size());
        uint32_t output_size_net = htonl(output_size);
        client_socket.send(&output_size_net, sizeof(output_size_net));
        client_socket.send(output.data(), output_size);

    } catch (const std::exception& e) {
        std::cerr << "Client handler error: " << e.what() << std::endl;
    }
}
