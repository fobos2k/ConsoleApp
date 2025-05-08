#include "logger.hpp"

#include <iostream>
#include <sstream>

#include "resources/logger.hpp"
#include "time_utils.hpp"

const std::unordered_map<Logger::LogLevel, Logger::LogLevelInfo>
    Logger::log_level_info_ = {
        {Logger::LogLevel::kDebug,
         {.text = resources::log_levels::kDebugText,
          .color_code = resources::log_levels::kDebugColor}},
        {Logger::LogLevel::kInfo,
         {.text = resources::log_levels::kInfoText,
          .color_code = resources::log_levels::kInfoColor}},
        {Logger::LogLevel::kWarning,
         {.text = resources::log_levels::kWarningText,
          .color_code = resources::log_levels::kWarningColor}},
        {Logger::LogLevel::kError,
         {.text = resources::log_levels::kErrorText,
          .color_code = resources::log_levels::kErrorColor}},
        {Logger::LogLevel::kFault,
         {.text = resources::log_levels::kFaultText,
          .color_code = resources::log_levels::kFaultColor}},
};

Logger::Logger() {
    start_time_ = std::chrono::steady_clock::now();
}

auto Logger::GetInstance() -> Logger & {
    static Logger instance;
    return instance;
}

void Logger::SetLevel(LogLevel level) {
    log_level_ = level;
}

void Logger::SetRelativeTime(bool relative) {
    use_relative_time_ = relative;
}

void Logger::SetLogFile(const std::string &filename) {
    std::lock_guard lock(mutex_);
    if (log_file_.is_open()) {
        log_file_.close();
    }
    log_file_.open(filename, std::ios::app);
}

auto Logger::GetLevelText(LogLevel level) -> std::string_view {
    auto level_entry = log_level_info_.find(level);
    return level_entry != log_level_info_.end() ? level_entry->second.text
                                                : "???";
}

auto Logger::GetLevelColor(LogLevel level) -> std::string_view {
    auto level_entry = log_level_info_.find(level);
    return level_entry != log_level_info_.end() ? level_entry->second.color_code
                                                : "";
}

auto Logger::GetTimestamp() const -> std::string {
    return use_relative_time_ ? common::time::SinceAppStart()
                              : common::time::CurrentDateTime();
}

void Logger::Log(LogLevel level, const std::string &message) {
    if (level < log_level_) {
        return;
    }

    std::lock_guard lock(mutex_);

    bool highlight_full_message =
        (level == LogLevel::kError || level == LogLevel::kFault);

    std::ostringstream output_stream;

    // Timestamp
    output_stream << resources::log_levels::kTimestampColor << "["
                  << GetTimestamp() << "]" << resources::log_levels::kResetColor
                  << " ";

    // Level
    output_stream << GetLevelColor(level) << "[" << GetLevelText(level) << "]"
                  << resources::log_levels::kResetColor << " ";

    // Message
    if (highlight_full_message) {
        output_stream << resources::log_levels::kBoldRedColor;
    }

    output_stream << message;

    if (highlight_full_message) {
        output_stream << resources::log_levels::kResetColor;
    }

    std::string final_message = output_stream.str();

    std::cout << final_message << '\n';

    if (log_file_.is_open()) {
        log_file_ << final_message << '\n' << std::flush;
    }
}
