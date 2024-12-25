#pragma once

#include "tcp_socket.hpp"
#include <string>

class Client {
public:
    Client() = default;
    void connect(const std::string& address, int port);

    std::string send_command(const std::string& command);

private:
    TCPSocket socket_;
};
