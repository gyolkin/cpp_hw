#pragma once

#include "socket.hpp"

class TCPSocket : public Socket {
public:
    TCPSocket();
    explicit TCPSocket(int fd);
    TCPSocket(TCPSocket&& other) noexcept;

    void listen(int backlog = SOMAXCONN);
    TCPSocket accept(std::string& client_address, uint16_t& client_port);
    void connect(const std::string& address, uint16_t port);

    size_t send(const void* data, size_t length);
    size_t receive(void* buffer, size_t length);
};
