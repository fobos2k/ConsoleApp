#include "cmdline_parser.hpp"
#include "resources/cmdline_parser.hpp"

#include <filesystem>
#include <iostream>

void CmdLineParser::ShowHelpAndExit() {
    std::cout
        << "Usage: consoleapp [options]\n\n"
        << "Options:\n"
        << "  -v, --verbose          Enable verbose logging (debug level)\n"
        << "  -r, --relative         Use relative timestamps in logs\n"
        << "  -s, --config <path>    Specify path to configuration file\n"
        << "  -h, --help             Show this help message\n";
    std::exit(EXIT_SUCCESS);
}

auto CmdLineParser::GetConfigPath() const -> std::string {
    LOG_INFO(resources::cmdline_parser::kTryingToLoadConfig, config_path_);

    std::filesystem::path path(config_path_);

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error(resources::cmdline_parser::kConfigNotFound +
                                 config_path_);
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(
            resources::cmdline_parser::kConfigCannotBeOpened + config_path_);
    }

    return config_path_;
}

void CmdLineParser::InitializeActions() {
    actions_ = {
        {"-v",
         [&](std::size_t &, const std::vector<std::string_view> &) {
             Logger::GetInstance().SetLevel(LogLevel::kDebug);
         }},
        {"--verbose",
         [&](std::size_t &, const std::vector<std::string_view> &) {
             Logger::GetInstance().SetLevel(LogLevel::kDebug);
         }},

        {"-h",
         [&](std::size_t &, const std::vector<std::string_view> &) {
             ShowHelpAndExit();
         }},
        {"--help",
         [&](std::size_t &, const std::vector<std::string_view> &) {
             ShowHelpAndExit();
         }},

        {"-r",
         [&](std::size_t &, const std::vector<std::string_view> &) {
             Logger::GetInstance().SetRelativeTime(true);
         }},
        {"--relative",
         [&](std::size_t &, const std::vector<std::string_view> &) {
             Logger::GetInstance().SetRelativeTime(true);
         }},

        {"-s",
         [&](std::size_t &pos, const std::vector<std::string_view> &args) {
             if (pos + 1 >= args.size()) {
                 throw std::runtime_error(
                     resources::cmdline_parser::kMissingArgumentConfig);
             }
             config_path_ = std::string(args[++pos]);
         }},
        {"--config",
         [&](std::size_t &pos, const std::vector<std::string_view> &args) {
             if (pos + 1 >= args.size()) {
                 throw std::runtime_error(
                     resources::cmdline_parser::kMissingArgumentConfig);
             }
             config_path_ = std::string(args[++pos]);
         }}};
}

void CmdLineParser::Parse(const std::vector<std::string_view> &args) {
    for (std::size_t i = 1; i < args.size(); ++i) {
        auto it_action = actions_.find(args[i]);
        if (it_action != actions_.end()) {
            it_action->second(i, args);
        } else {
            throw std::runtime_error(
                std::string(resources::cmdline_parser::kUnknownArgument) +
                std::string(args[i]));
        }
    }
}
