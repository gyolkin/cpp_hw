#include "keylogger.hpp"
#include <chrono>
#include <iostream>
#include <random>

Keylogger::Keylogger()
    : running_(false)
{}

Keylogger::~Keylogger() {
    stop();
}

void Keylogger::start() {
    if (running_) return;
    running_ = true;
    thread_ = std::thread(&Keylogger::captrure, this);
}

void Keylogger::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

std::string Keylogger::flush_buffer() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::string result = buffer_;
    buffer_.clear();
    return result;
}

size_t Keylogger::buffer_size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return buffer_.size();
}

/* псевдо захват клавиатуры */
void Keylogger::captrure() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> key_dist(0, 25);
    std::uniform_int_distribution<int> time_dist(200, 600);
    while (running_) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_ += 'a' + key_dist(gen);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(time_dist(gen)));
    }
}
