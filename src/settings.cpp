#include "settings.hpp"

#include <filesystem>
#include <stdexcept>

#include "logger.hpp"

Settings::Settings(const std::string &path) {

    LOG_INFO("Loading settings from: {}", path);

    std::filesystem::path config_path(path);
    if (!std::filesystem::exists(config_path)) {
        throw std::runtime_error("Config file not found: " + path);
    }

    std::ifstream file(config_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + path);
    }

    Parse(file); // читаем и парсим настройки
}

void Settings::Parse(std::ifstream &file) {
    // Здесь будет реальный парсер конфигурации
    // Пока можно просто прочитать строки для примера
    std::string line;
    while (std::getline(file, line)) {
        LOG_DEBUG("Read config line: {}", line);
    }
}
