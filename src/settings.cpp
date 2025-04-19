#include "settings.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "logger.hpp"
#include "resources.hpp"

Settings::Settings(const std::string &path) {
    namespace fs = std::filesystem;

    std::string final_path = path;
    LOG_INFO("Loading settings from: {}", final_path);

    fs::path config_path(final_path);

    if (!fs::exists(config_path)) {
        LOG_WARNING("Config file '{}' not found, trying default '{}'",
                    final_path, resources::settings::kDefaultConfigFilename);
        final_path = resources::settings::kDefaultConfigFilename;
        config_path = fs::path(final_path);
    }

    if (!fs::exists(config_path)) {
        throw std::runtime_error(
            "Neither specified nor default config file found: " + final_path);
    }

    std::ifstream file(config_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + final_path);
    }

    LOG_INFO(resources::settings::kUsingConfigFile, final_path);

    Parse(file); // читаем и парсим настройки
}

void Settings::Parse(std::ifstream &file) {
    std::string line;
    while (std::getline(file, line)) {
        LOG_DEBUG("Read config line: {}", line);
    }
}
