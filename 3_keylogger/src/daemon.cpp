#include "daemon.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <filesystem>

bool daemonize(std::optional<std::string> log_file) {
    pid_t pid = ::fork();

    if (pid < 0) {
        std::cerr << "Failed to create a new process (first fork): " << std::strerror(errno) << std::endl;
        return false;
    }
    if (pid > 0) {
        std::exit(EXIT_SUCCESS);
    }

    if (::setsid() < 0) {
        std::cerr << "Failed to create a new session: " << std::strerror(errno) << std::endl;
        return false;
    }

    pid = ::fork();
    if (pid < 0) {
        std::cerr << "Failed to create a new process (second fork): " << std::strerror(errno) << std::endl;
        return false;
    }
    if (pid > 0) {
        std::exit(EXIT_SUCCESS);
    }

    ::umask(0);

    if (::chdir("/") < 0) {
        std::cerr << "Failed to change directory: " << std::strerror(errno) << std::endl;
        return false;
    }

    if (log_file) {
        std::filesystem::path log_path = "/tmp/" + *log_file;
        int fd = ::open(log_path.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU | S_IRGRP | S_IROTH);
        if (fd < 0) {
            std::cerr << "Failed to open log file " << log_path << ": " << std::strerror(errno) << std::endl;
            return false;
        }
        if (::dup2(fd, STDOUT_FILENO) < 0 || ::dup2(fd, STDERR_FILENO) < 0) {
            std::cerr << "Failed to redirect output to " << log_path << ": " << std::strerror(errno) << std::endl;
            ::close(fd);
            return false;
        }
        ::close(fd);
    } else {
        int dev_null = ::open("/dev/null", O_RDWR);
        if (dev_null < 0) {
            std::cerr << "Failed to open /dev/null: " << std::strerror(errno) << std::endl;
            return false;
        }
        if (::dup2(dev_null, STDIN_FILENO) < 0 ||
            ::dup2(dev_null, STDOUT_FILENO) < 0 ||
            ::dup2(dev_null, STDERR_FILENO) < 0) {
            std::cerr << "Failed to redirect fds to /dev/null: " << std::strerror(errno) << std::endl;
            ::close(dev_null);
            return false;
        }
        ::close(dev_null);
    }
    return true;
}
