#pragma once

#include "udp_socket.hpp"
#include "socket_error.hpp"
#include <string>
#include <thread>

class Client {
public:
    explicit Client(const std::string& username = "anonymous");
    ~Client();

    void start(int port);
    void stop();

private:
    UDPSocket socket_;
    std::string username_;
    bool running_;
    int port_;
    std::thread receive_thread_;

    void read_input();
    void receive_messages();
    void handle_command(const std::string& command);
    void broadcast_message(const std::string& message);

    void cmd_help() const;
    void cmd_name(const std::string& new_name);
    void cmd_quit();

    static constexpr size_t MAX_MESSAGE_SIZE = 1024;
};
