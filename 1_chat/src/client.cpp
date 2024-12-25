#include "client.hpp"
#include <iostream>
#include <sstream>

Client::Client(const std::string& username)
    : socket_()
    , username_(username)
    , running_(false) {
}

Client::~Client() {
    stop();
}

void Client::start(int port) {
    try {
        socket_.set_broadcast(true);
        socket_.set_reuse_address(true);
        socket_.bind("0.0.0.0", static_cast<uint16_t>(port));
        running_ = true;
        receive_thread_ = std::thread(&Client::receive_messages, this);
        std::cout << "Chat client started. Type /help for available commands.\n";
        read_input();
    } catch (const SocketError& e) {
        std::cerr << "Failed to start client: " << e.what() << std::endl;
    }
}

void Client::stop() {
    if (running_) {
        running_ = false;
        if (receive_thread_.joinable()) {
            receive_thread_.join();
        }
    }
}

void Client::read_input() {
    std::string input;
    while (running_ && std::getline(std::cin, input)) {
        if (input.empty()) {
            continue;
        }
        if (input[0] == '/') {
            handle_command(input);
        } else {
            broadcast_message(input);
        }
    }
}

void Client::receive_messages() {
    char buffer[MAX_MESSAGE_SIZE];
    std::string sender_address;
    uint16_t sender_port;
    while (running_) {
        try {
            size_t received = socket_.receive_from(buffer, MAX_MESSAGE_SIZE - 1, sender_address, sender_port);
            buffer[received] = '\0';
            std::cout << buffer << std::endl;
        } catch (const SocketError& e) {
            if (running_) {
                std::cerr << "Error receiving message: " << e.what() << std::endl;
            }
        }
    }
}

void Client::broadcast_message(const std::string& message) {
    try {
        std::string formatted = "[" + username_ + "]: " + message;
        socket_.send_to(formatted.c_str(), formatted.length(), "255.255.255.255", socket_.get_port());
    } catch (const SocketError& e) {
        std::cerr << "Failed to send message: " << e.what() << std::endl;
    }
}

void Client::handle_command(const std::string& command) {
    std::istringstream stream(command);
    std::string cmd;
    stream >> cmd;
    if (cmd == "/help") {
        cmd_help();
    } else if (cmd == "/name") {
        std::string new_name;
        stream >> new_name;
        cmd_name(new_name);
    } else if (cmd == "/quit") {
        cmd_quit();
    } else {
        std::cout << "Unknown command. Type /help for available commands.\n";
    }
}

void Client::cmd_help() const {
    std::cout << "Available commands:\n"
        << "  /help         - Show help message\n"
        << "  /name <name>  - Change your username\n"
        << "  /quit         - Exit the chat\n";
}

void Client::cmd_name(const std::string& new_name) {
    if (new_name.empty()) {
        std::cout << "Usage: /name <new_name>\n";
        return;
    }
    std::string old_name = username_;
    username_ = new_name;
    std::string message = "changed their name to " + new_name;
    broadcast_message(message);
}

void Client::cmd_quit() {
    std::string message = "has left the chat";
    broadcast_message(message);
    running_ = false;
}
