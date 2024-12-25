#include "socket_error.hpp"
#include <cerrno>
#include <cstring>

SocketError::SocketError(const std::string& message)
    : std::runtime_error(message) {}

SocketError::SocketError(const std::string& message, int error_code)
    : std::runtime_error(message + ": " + std::strerror(error_code)) {}
