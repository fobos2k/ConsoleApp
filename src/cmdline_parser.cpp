#include "cmdline_parser.hpp"

#include <filesystem>
#include <iostream>
#include <ostream>
#include <unordered_set>

#include "logger.hpp"
#include "resources/cmdline_parser.hpp"

CmdLineParser::CmdLineParser(const std::vector<std::string_view> &args) {
    program_name_ = args[0];
    InitializeActions();
    Parse(args);
}

auto CmdLineParser::IsHelpRequested() const -> bool {
    return help_requested_;
}

auto CmdLineParser::GetConfigPath() const -> std::string {
    LOG_INFO(resources::cmdline::kUsingConfigFile, config_path_);

    std::filesystem::path path(config_path_);

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error(resources::cmdline::kConfigNotFound +
                                 config_path_);
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error(resources::cmdline::kConfigCannotBeOpened +
                                 config_path_);
    }

    return config_path_;
}

void CmdLineParser::ShowHelp(std::ostream &ostr) const {
    std::string program_name_only =
        std::filesystem::path(program_name_).filename().string();
    auto options = CollectOptions();
    std::size_t max_opt_length = CalculateMaxOptionLength(options);

    ostr << "\nUsage: " << program_name_only << " [options]\n\n"
         << "Options:\n";

    PrintHelp(ostr, options, max_opt_length);
}

void CmdLineParser::InitializeActions() {

    RegisterAlias(
        "-s", "--config",
        [this](std::size_t &pos, const std::vector<std::string_view> &args) {
            if (pos + 1 >= args.size()) {
                throw std::runtime_error(
                    resources::cmdline::kMissingArgumentConfig);
            }
            config_path_ = std::string(args[++pos]);
        },
        "Specify config file path", true, "path_to_config");

    RegisterAlias(
        "-r", "--relative",
        [](std::size_t &, const std::vector<std::string_view> &) {
            Logger::GetInstance().SetRelativeTime(true);
        },
        "Use relative timestamps");

    RegisterAlias(
        "-v", "--verbose",
        [](std::size_t &, const std::vector<std::string_view> &) {
            Logger::GetInstance().SetLevel(Logger::LogLevel::kDebug);
        },
        "Enable verbose (debug) logging");

    RegisterAlias(
        "-h", "--help",
        [this](std::size_t &, const std::vector<std::string_view> &) {
            help_requested_ = true;
        },
        "Show help and exit");
}

auto CmdLineParser::CollectOptions() const -> OptionList {
    OptionList options;

    std::unordered_set<std::string_view> used;

    LOG_DEBUG("ordered_options_ = {}", ordered_options_);

    for (const auto &opt : ordered_options_) {
        auto it_act = actions_.find(opt);
        if (it_act == actions_.end()) {
            continue;
        }

        const auto &info = it_act->second;

        if (used.contains(opt)) {
            continue;
        }

        options.emplace_back(opt, info.short_opt, info.description,
                             info.requires_argument, info.argument_name);

        used.insert(opt);
        if (!info.short_opt.empty()) {
            used.insert(info.short_opt);
        }
    }

    LOG_DEBUG("options = {}", options);
    return options;
}

auto CmdLineParser::CalculateMaxOptionLength(const OptionList &options)
    -> std::size_t {
    std::size_t max_opt_length = 0;

    for (const auto &entry : options) {
        const auto &short_opt = std::get<0>(entry);
        const auto &long_opt = std::get<1>(entry);
        const auto &requires_arg = std::get<3>(entry);
        const auto &arg_name = std::get<4>(entry);

        std::size_t len = 2;
        len += (short_opt.empty() ? 4 : short_opt.size() + 2);
        len += long_opt.size();
        if (requires_arg) {
            len += 2;
            len += arg_name.has_value() ? arg_name->size() : 3;
        }
        max_opt_length = std::max(len, max_opt_length);
    }

    return max_opt_length;
}

void CmdLineParser::PrintHelp(std::ostream &output_stream,
                              const OptionList &options,
                              std::size_t max_opt_length) {
    for (const auto &[short_opt, long_opt, desc, requires_arg, arg_name] :
         options) {
        std::ostringstream opt_stream;

        opt_stream << "  ";
        if (!short_opt.empty()) {
            opt_stream << short_opt << ", ";
        } else {
            opt_stream << "    ";
        }

        opt_stream << long_opt;

        if (requires_arg) {
            if (arg_name.has_value()) {
                opt_stream << " <" << *arg_name << '>';
            } else {
                opt_stream << " <arg>";
            }
        }

        std::string opt_text = opt_stream.str();
        std::size_t len = opt_text.length();

        output_stream << opt_text;

        if (len < max_opt_length) {
            output_stream << std::string(max_opt_length - len + 1, ' ');
        }

        output_stream << "    " << desc << "\n";
    }
}

void CmdLineParser::Parse(const std::vector<std::string_view> &args) {
    for (std::size_t i = 1; i < args.size(); ++i) {
        auto it_action = actions_.find(args[i]);
        if (it_action != actions_.end()) {
            it_action->second.handler(i, args);
        } else {
            throw std::runtime_error(
                std::string(resources::cmdline::kUnknownArgument) +
                std::string(args[i]));
        }
    }
}
