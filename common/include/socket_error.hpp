#pragma once

#include <stdexcept>
#include <string>

class SocketError : public std::runtime_error {
public:
    explicit SocketError(const std::string& message);
    explicit SocketError(const std::string& message, int error_code);
};
