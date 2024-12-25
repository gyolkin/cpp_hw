#pragma once

#include "tcp_socket.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <fstream>

class Server {
public:
    Server(int port);
    ~Server();

    bool start();
    void stop();

private:
    void run();
    void handle_client(TCPSocket client_socket, const std::string& address, int port);

    int port_;
    TCPSocket server_socket_;
    bool running_;
    std::thread thread_;
    std::mutex mutex_;
    std::unordered_map<std::string, std::ofstream*> client_files_;

    static constexpr size_t MAX_BUFFER_SIZE = 1024;
};
