#include "client.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <stdexcept>

void Client::connect(const std::string& address, int port) {
    socket_.connect(address, static_cast<uint16_t>(port));
}

std::string Client::send_command(const std::string& command) {
    uint32_t cmd_len = static_cast<uint32_t>(command.size());
    uint32_t cmd_len_net = htonl(cmd_len);
    socket_.send(&cmd_len_net, sizeof(cmd_len_net));
    socket_.send(command.data(), cmd_len);

    uint32_t out_len_net;
    size_t received = socket_.receive(&out_len_net, sizeof(out_len_net));
    if (received < sizeof(out_len_net)) {
        throw std::runtime_error("Server closed connection");
    }
    uint32_t out_len = ntohl(out_len_net);

    std::vector<char> out_buffer(out_len + 1, 0);
    received = 0;
    while (received < out_len) {
        size_t received_bytes = socket_.receive(out_buffer.data() + received, out_len - received);
        if (received_bytes == 0) {
            throw std::runtime_error("Server closed connection");
        }
        received += received_bytes;
    }

    return std::string(out_buffer.data(), out_len);
}
