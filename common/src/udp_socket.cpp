#include "udp_socket.hpp"
#include "socket_error.hpp"

UDPSocket::UDPSocket() {
    create_socket(AF_INET, SOCK_DGRAM, 0);
}

void UDPSocket::set_broadcast(bool enable) {
    int opt = enable ? 1 : 0;
    set_option(SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
}

void UDPSocket::send_to(const void* data, size_t length, const std::string& address, uint16_t port) {
    sockaddr_in addr = create_address(address, port);
    ssize_t sent = sendto(socket_fd_, data, length, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (sent < 0) {
        throw SocketError("send_to failed", errno);
    }
}

size_t UDPSocket::receive_from(void* buffer, size_t length, std::string& address, uint16_t& port) {
    sockaddr_in sender_addr{};
    socklen_t addr_len = sizeof(sender_addr);
    ssize_t received = recvfrom(socket_fd_, buffer, length, 0, (struct sockaddr*)&sender_addr, &addr_len);
    if (received < 0) {
        throw SocketError("receive_from failed", errno);
    }
    char addr_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &sender_addr.sin_addr, addr_str, sizeof(addr_str)) == nullptr) {
        throw SocketError("Failed to convert sender address", errno);
    }
    address = addr_str;
    port = ntohs(sender_addr.sin_port);
    return static_cast<size_t>(received);
}
