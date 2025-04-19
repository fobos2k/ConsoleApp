
#include "cmdline_parser.hpp"
#include "logger.hpp"
#include "resources.hpp"
#include "settings.hpp"

#include <cstdlib>
#include <vector>

static auto ToStringViews(int argc, char **argv)
    -> std::vector<std::string_view> {
    std::vector<std::string_view> result;
    result.reserve(static_cast<std::size_t>(argc));
    for (int i = 0; i < argc; ++i) {
        result.emplace_back(argv[i]);
    }
    return result;
}

auto main(int argc, char *argv[]) -> int {
    try {
        auto args = ToStringViews(argc, argv);

        CmdLineParser parser(args);

        Settings settings(parser.GetConfigPath());

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        LOG_ERROR(resources::kFatalExceptionCaught, e.what());
        return EXIT_FAILURE;
    } catch (...) {
        LOG_ERROR(resources::kUnknownFatalExceptionCaught);
        return EXIT_FAILURE;
    }
}
