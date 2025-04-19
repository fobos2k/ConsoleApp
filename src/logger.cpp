#include "logger.hpp"
#include "time_utils.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_map>

auto Logger::LevelToString(LogLevel level) -> std::string {
    static const std::unordered_map<LogLevel, std::string> level_map = {
        {LogLevel::kDebug, "DBG"},
        {LogLevel::kInfo, "INF"},
        {LogLevel::kWarning, "WRN"},
        {LogLevel::kError, "ERR"},
        {LogLevel::kFault, "FLT"}};

    auto it_level = level_map.find(level);
    return it_level != level_map.end() ? it_level->second : "???";
}

auto Logger::GetTimeStamp() const -> std::string {
    std::string timestamp = use_relative_time_
                                ? common::time::SinceAppStart()
                                : common::time::CurrentDateTime();

    return timestamp;
}

void Logger::SetLogFile(const std::string &filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (log_file_.is_open()) {
        log_file_.close();
    }
    log_file_.open(filename, std::ios::app);
}

void Logger::Log(LogLevel level, const std::string &message) {
    if (level < log_level_) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    std::ostringstream oss;
    oss << GetTimeStamp() << " [" << LevelToString(level) << "] " << message;

    std::cout << oss.str() << '\n';
    if (log_file_.is_open()) {
        log_file_ << oss.str() << "\n" << std::flush;
    }
}
