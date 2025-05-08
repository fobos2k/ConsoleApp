#include "settings.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string_view>

#include "logger.hpp"
#include "resources/settings.hpp"

Settings::Settings(const std::string &path) {
    namespace fs = std::filesystem;

    std::string final_path = path;

    fs::path config_path(final_path);

    if (!fs::exists(config_path)) {
        LOG_WARNING(resources::settings::kConfigFileNotFound, final_path,
                    resources::settings::kDefaultConfigFilename);
        final_path = resources::settings::kDefaultConfigFilename;
        config_path = fs::path(final_path);
    }

    if (!fs::exists(config_path)) {
        throw std::runtime_error(resources::settings::kDefaultConfigNotFound +
                                 final_path);
    }

    std::ifstream file(config_path);
    if (!file.is_open()) {
        throw std::runtime_error(resources::settings::kCanNotOpenConfigFile +
                                 final_path);
    }

    LOG_INFO(resources::settings::kLoadingSettingsFrom, final_path);

    Parse(file);
}

void Settings::Parse(std::ifstream &file) {
    std::string line;
    while (std::getline(file, line)) {
        LOG_DEBUG("Read config line: {}", line);

        // Простейший парсер "key = value"
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            continue; // просто пропускаем неправильные строки
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Убираем возможные пробелы вокруг
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);

        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (!key.empty()) {
            settings_[key] = value;
        }
    }
}

auto Settings::GetString(const std::string &key) const -> std::string {
    auto its = settings_.find(key);
    if (its == settings_.end()) {
        throw std::runtime_error("Missing configuration key: " + key);
    }
    return its->second;
}

auto Settings::GetStringOrDefault(const std::string &key,
                                  std::string_view &default_value) const
    -> std::string {
    auto its = settings_.find(key);
    return its != settings_.end() ? its->second : std::string(default_value);
}

auto Settings::GetInt(const std::string &key) const -> int {
    return std::stoi(GetString(key));
}

auto Settings::GetIntOrDefault(const std::string &key, int default_value) const
    -> int {
    auto its = settings_.find(key);
    return its != settings_.end() ? std::stoi(its->second) : default_value;
}

auto Settings::GetFloat(const std::string &key) const -> float {
    return std::stof(GetString(key));
}

auto Settings::GetFloatOrDefault(const std::string &key,
                                 float default_value) const -> float {
    auto its = settings_.find(key);
    return its != settings_.end() ? std::stof(its->second) : default_value;
}

auto Settings::GetDouble(const std::string &key) const -> double {
    return std::stod(GetString(key));
}

auto Settings::GetDoubleOrDefault(const std::string &key,
                                  double default_value) const -> double {
    auto its = settings_.find(key);
    return its != settings_.end() ? std::stod(its->second) : default_value;
}

auto Settings::GetBoolOrDefault(const std::string &key,
                                bool default_value) const -> bool {
    // auto its = settings_.find(key);
    // if (std::iter_swap != settings_.end()) {
    //     std::string val = its->second;
    //     for (auto &chr : val)
    //         chr = static_cast<char>(std::tolower(chr));

    //     return (val == "true" || val == "1" || val == "yes" || val == "on");
    // }
    return default_value;
}
