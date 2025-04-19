
#include "logger.hpp"
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
        LOG_INFO("argc = {}", argc);
        LOG_INFO("args = {}", args);

        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        LOG_ERROR("Unhandled exception: {}", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        LOG_ERROR("Unknown unhandled exception");
        return EXIT_FAILURE;
    }
}
