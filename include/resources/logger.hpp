#ifndef CONSOLEAPP_RESOURCES_LOGGER_HPP
#define CONSOLEAPP_RESOURCES_LOGGER_HPP

#include <string_view>

//===================
// Log level strings and colors
//===================
namespace resources::log_levels {

inline constexpr std::string_view kResetColor = "\033[0m";

inline constexpr std::string_view kTimestampColor = "\033[32m"; // Bright green
inline constexpr std::string_view kBoldRedColor = "\033[1;31m";

inline constexpr std::string_view kDebugText = "DBG";
inline constexpr std::string_view kDebugColor = "\033[90m"; // Bright gray

inline constexpr std::string_view kInfoText = "INF";
inline constexpr std::string_view kInfoColor = "\033[37m"; // White

inline constexpr std::string_view kWarningText = "WRN";
inline constexpr std::string_view kWarningColor = "\033[33m"; // Yellow

inline constexpr std::string_view kErrorText = "ERR";
inline constexpr std::string_view kErrorColor = "\033[1;31m"; // Red

inline constexpr std::string_view kFaultText = "FLT";
inline constexpr std::string_view kFaultColor = "\033[1;35m"; // Magenta

} // namespace resources::log_levels

#endif // CONSOLEAPP_RESOURCES_LOGGER_HPP
