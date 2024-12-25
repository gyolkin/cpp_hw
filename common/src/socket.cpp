#include "socket.hpp"
#include "socket_error.hpp"

Socket::Socket() : socket_fd_(-1) {}

Socket::~Socket() {
    close();
}

bool Socket::is_open() const noexcept {
    return socket_fd_ != -1;
}

int Socket::get_fd() const noexcept {
    return socket_fd_;
}

uint16_t Socket::get_port() const noexcept {
    return port_;
}

void Socket::close() {
    if (is_open()) {
        ::close(socket_fd_);
        socket_fd_ = -1;
    }
}

void Socket::set_reuse_address(bool enable) {
    int opt = enable ? 1 : 0;
    set_option(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Socket::create_socket(int domain, int type, int protocol) {
    socket_fd_ = ::socket(domain, type, protocol);
    if (socket_fd_ == -1) {
        throw SocketError("Failed to create socket", errno);
    }
}

void Socket::bind(const std::string& address, uint16_t port) {
    sockaddr_in addr = create_address(address, port);
    port_ = port;
    if (::bind(socket_fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw SocketError("Failed to bind", errno);
    }
}

void Socket::set_option(int level, int option_name, const void* option_value, socklen_t option_len) {
    if (setsockopt(socket_fd_, level, option_name, option_value, option_len) < 0) {
        throw SocketError("Failed to set socket option", errno);
    }
}

sockaddr_in Socket::create_address(const std::string& address, uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) <= 0) {
        throw SocketError("Invalid address: " + address);
    }
    return addr;
}
