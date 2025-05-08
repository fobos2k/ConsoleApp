#include <cstdlib>
#include <span>
#include <string_view>
#include <vector>

#include "cmdline_parser.hpp"
#include "logger.hpp"
#include "resources.hpp"

auto ToStringViews(std::span<char *> args) -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    result.reserve(args.size());

    for (auto *arg : args) {
        result.emplace_back(arg);
    }

    return result;
}

auto Start(std::vector<std::string_view> &args) {
    try {

        CmdLineParser parser(args);

        if (parser.IsHelpRequested()) {
            parser.ShowHelp();
            return EXIT_SUCCESS;
        }

        LOG_DEBUG("Parse settings: {}", parser.GetConfigPath());
        // Settings settings(parser.GetConfigPath());

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        LOG_ERROR(resources::common::kFatalExceptionCaught, e.what());
        return EXIT_FAILURE;
    } catch (...) {
        LOG_ERROR(resources::common::kUnknownFatalExceptionCaught);
        return EXIT_FAILURE;
    }
}

auto main(int argc, char *argv[]) -> int {
    auto args = ToStringViews(std::span(argv, static_cast<std::size_t>(argc)));
    return Start(args);
}
