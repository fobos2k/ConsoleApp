#ifndef CONSOLEAPP_RESOURCES_SETTINGS_HPP
#define CONSOLEAPP_RESOURCES_SETTINGS_HPP

#include <string>

namespace resources::settings {

inline const std::string
    kDefaultConfigFilename = "config.yml",
    kCanNotOpenConfigFile = "Cannot open config file: ",
    kLoadingSettingsFrom = "Loading settings from: {}",
    kConfigFileNotFound = "Config file '{}' not found, trying default '{}'",
    kDefaultConfigNotFound =
        "Neither specified nor default config file found: ",
    kReadConfigLine = "Read config line: {}";
} // namespace resources::settings

#endif // CONSOLEAPP_RESOURCES_SETTINGS_HPP
