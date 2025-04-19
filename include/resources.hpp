#ifndef CONSOLEAPP_RESOURCES_HPP
#define CONSOLEAPP_RESOURCES_HPP

#include <string>

namespace resources {

#include "resources/cmdline_parser.hpp"
#include "resources/settings.hpp"
#include "resources/tinyfmt.hpp"

// main.cpp
inline const std::string kFatalExceptionCaught = "Fatal exception caught: {}";
inline const std::string kUnknownFatalExceptionCaught =
    "Unknown fatal exception caught";

} // namespace resources

#endif // CONSOLEAPP_RESOURCES_HPP
