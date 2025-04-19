#ifndef CONSOLEAPP_SETTINGS_HPP
#define CONSOLEAPP_SETTINGS_HPP

#include <fstream>
#include <string>

class Settings {
  public:
    explicit Settings(const std::string &path);

    // Здесь будут методы для доступа к настройкам:
    // std::string GetString(const std::string& key) const;
    // int GetInt(const std::string& key) const;
    // и т.д.

  private:
    static void Parse(std::ifstream &file);
};

#endif // CONSOLEAPP_SETTINGS_HPP
