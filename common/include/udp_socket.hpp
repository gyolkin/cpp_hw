#pragma once

#include "socket.hpp"

class UDPSocket : public Socket {
public:
    UDPSocket();

    void set_broadcast(bool enable);
    void send_to(const void* data, size_t length, const std::string& address, uint16_t port);
    size_t receive_from(void* buffer, size_t length, std::string& address, uint16_t& port);
};
