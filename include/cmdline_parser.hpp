#ifndef CONSOLEAPP_CMDLINE_PARSER_HPP
#define CONSOLEAPP_CMDLINE_PARSER_HPP

#include <functional>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "resources/settings.hpp"

class CmdLineParser {
  public:
    explicit CmdLineParser(const std::vector<std::string_view> &args);

    auto IsHelpRequested() const -> bool;
    auto GetConfigPath() const -> std::string;

    void ShowHelp(std::ostream &ostr = std::cout) const;

  private:
    using OptionEntry =
        std::tuple<std::string_view,               // short_opt
                   std::string_view,               // long_opt
                   std::string,                    // description
                   bool,                           // requires_argument
                   std::optional<std::string_view> // argument_name
                   >;

    using OptionList = std::vector<OptionEntry>;

    std::string program_name_;
    std::string config_path_ = resources::settings::kDefaultConfigFilename;
    bool help_requested_ = false;

    struct ActionInfo {
        std::function<void(std::size_t &,
                           const std::vector<std::string_view> &)>
            handler;
        std::string description;
        std::string_view short_opt;
        bool requires_argument = false;
        std::optional<std::string_view> argument_name;
    };

    std::unordered_map<std::string_view, ActionInfo> actions_;
    std::vector<std::string_view> ordered_options_;

    void InitializeActions();

    auto CollectOptions() const -> OptionList;
    static auto CalculateMaxOptionLength(const OptionList &options)
        -> std::size_t;
    static void PrintHelp(std::ostream &output_stream,
                          const OptionList &options,
                          std::size_t max_opt_length);

    void Parse(const std::vector<std::string_view> &args);

    template <typename Handler>
    void RegisterAlias(
        std::string_view short_opt, std::string_view long_opt,
        Handler &&handler, std::string_view description,
        bool requires_argument = false,
        std::optional<std::string_view> argument_name = std::nullopt) {
        auto hdl = std::forward<Handler>(handler);

        actions_[short_opt] = {hdl, std::string(description), long_opt,
                               requires_argument, argument_name};
        actions_[long_opt] = {std::move(hdl), std::string(description),
                              short_opt, requires_argument, argument_name};

        ordered_options_.push_back(short_opt);
    }
};

#endif // CONSOLEAPP_CMDLINE_PARSER_HPP
