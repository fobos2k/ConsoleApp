#ifndef CONSOLEAPP_RESOURCES_CMDLINE_PARSER_HPP
#define CONSOLEAPP_RESOURCES_CMDLINE_PARSER_HPP

#include <string>

namespace cmdline_parser {

inline const std::string kConfigCannotBeOpened =
    "Config file exists but cannot be opened: ";
inline const std::string kConfigNotFound = "Config file not found: ";
inline const std::string kMissingArgumentConfig =
    "Missing argument for --config";
inline const std::string kUsingConfigFile = "Trying to load config file: {}";
inline const std::string kUnknownArgument = "Unknown argument: ";

} // namespace cmdline_parser

#endif // CONSOLEAPP_RESOURCES_CMDLINE_PARSER_HPP
