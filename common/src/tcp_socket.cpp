#include "tcp_socket.hpp"
#include "socket_error.hpp"

TCPSocket::TCPSocket() {
    create_socket(AF_INET, SOCK_STREAM, 0);
}

TCPSocket::TCPSocket(int fd) {
    socket_fd_ = fd;
}

TCPSocket::TCPSocket(TCPSocket&& other) noexcept {
    socket_fd_ = other.socket_fd_;
    other.socket_fd_ = -1;
}

void TCPSocket::listen(int backlog) {
    if (::listen(socket_fd_, backlog) < 0) {
        throw SocketError("Failed to listen", errno);
    }
}

TCPSocket TCPSocket::accept(std::string& client_address, uint16_t& client_port) {
    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = ::accept(socket_fd_, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        throw SocketError("Failed to accept connection", errno);
    }
    char addr_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &client_addr.sin_addr, addr_str, sizeof(addr_str)) == nullptr) {
        throw SocketError("Failed to convert client address", errno);
    }
    client_address = addr_str;
    client_port = ntohs(client_addr.sin_port);

    return TCPSocket(client_fd);
}

void TCPSocket::connect(const std::string& address, uint16_t port) {
    sockaddr_in addr = create_address(address, port);
    if (::connect(socket_fd_, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        throw SocketError("Failed to connect", errno);
    }
}

size_t TCPSocket::send(const void* data, size_t length) {
    ssize_t sent = ::send(socket_fd_, data, length, 0);
    if (sent < 0) {
        throw SocketError("Failed to send", errno);
    }
    return static_cast<size_t>(sent);
}

size_t TCPSocket::receive(void* buffer, size_t length) {
    ssize_t received = ::recv(socket_fd_, buffer, length, 0);
    if (received < 0) {
        throw SocketError("Failed to receive", errno);
    }
    return static_cast<size_t>(received);
}
