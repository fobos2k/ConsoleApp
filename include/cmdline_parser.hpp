#ifndef CONSOLEAPP_CMDLINE_PARSER_HPP
#define CONSOLEAPP_CMDLINE_PARSER_HPP

#include "logger.hpp"
#include "resources.hpp"

#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "resources/settings.hpp"

class CmdLineParser {
  public:
    explicit CmdLineParser(const std::vector<std::string_view> &args) {
        InitializeActions();
        Parse(args);
    }

    auto GetConfigPath() const -> std::string;

  private:
    std::string config_path_ = resources::settings::kDefaultConfigFilename;

    std::unordered_map<
        std::string_view,
        std::function<void(std::size_t &,
                           const std::vector<std::string_view> &)>>
        actions_;

    void InitializeActions();
    static void ShowHelpAndExit();
    void Parse(const std::vector<std::string_view> &args);
};

#endif // CONSOLEAPP_CMDLINE_PARSER_HPP
