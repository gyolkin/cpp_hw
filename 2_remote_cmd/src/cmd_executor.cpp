#include "cmd_executor.hpp"
#include <cstdio>
#include <array>
#include <memory>

std::string CommandExecutor::execute(const std::string& command) {
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Failed to run command\n";
    }
    std::array<char, 128> buffer{};
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result;
}
