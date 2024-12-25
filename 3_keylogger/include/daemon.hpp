#pragma once

#include <optional>
#include <string>

bool daemonize(std::optional<std::string> log_file = std::nullopt);
