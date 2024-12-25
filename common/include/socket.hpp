#pragma once

#include <string>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

class Socket {
public:
    virtual ~Socket();
    bool is_open() const noexcept;
    int get_fd() const noexcept;
    uint16_t get_port() const noexcept;
    void close();
    void bind(const std::string& address, uint16_t port);
    void set_reuse_address(bool enable);

protected:
    Socket();

    int socket_fd_;
    uint16_t port_;

    void create_socket(int domain, int type, int protocol = 0);
    void set_option(int level, int option_name, const void* option_value, socklen_t option_len);
    static sockaddr_in create_address(const std::string& address, uint16_t port);
};
