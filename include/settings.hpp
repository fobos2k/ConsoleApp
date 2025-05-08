#ifndef CONSOLEAPP_SETTINGS_HPP
#define CONSOLEAPP_SETTINGS_HPP

#include <fstream>
#include <string>
#include <unordered_map>

class Settings {
  public:
    explicit Settings(const std::string &path);

    auto GetString(const std::string &key) const -> std::string;
    auto GetStringOrDefault(const std::string &key,
                            std::string_view &default_value) const
        -> std::string;

    auto GetInt(const std::string &key) const -> int;
    auto GetIntOrDefault(const std::string &key, int default_value) const
        -> int;

    auto GetFloat(const std::string &key) const -> float;
    auto GetFloatOrDefault(const std::string &key, float default_value) const
        -> float;

    auto GetDouble(const std::string &key) const -> double;
    auto GetDoubleOrDefault(const std::string &key, double default_value) const
        -> double;

    auto GetBool(const std::string &key) const -> bool;
    auto GetBoolOrDefault(const std::string &key, bool default_value) const
        -> bool;

  private:
    std::unordered_map<std::string, std::string> settings_;
    void Parse(std::ifstream &file);
};

#endif // CONSOLEAPP_SETTINGS_HPP
