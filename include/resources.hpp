#ifndef CONSOLEAPP_RESOURCES_HPP
#define CONSOLEAPP_RESOURCES_HPP

#include <string>

namespace resources::common {

// main.cpp
inline const std::string kFatalExceptionCaught = "Fatal exception caught: {}";
inline const std::string kUnknownFatalExceptionCaught =
    "Unknown fatal exception caught";

} // namespace resources::common

#endif // CONSOLEAPP_RESOURCES_HPP
