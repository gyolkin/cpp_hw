#pragma once

#include "tcp_socket.hpp"
#include "cmd_executor.hpp"
#include <string>
#include <memory>

class Server {
public:
    Server(const std::string& address, int port, std::unique_ptr<CommandExecutor> executor);
    ~Server();

    void start();
    void stop();

private:
    void handle_client(TCPSocket client_socket);

    TCPSocket listening_socket_;
    bool running_;
    std::unique_ptr<CommandExecutor> command_executor_;
};
