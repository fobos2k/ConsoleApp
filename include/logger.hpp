#ifndef CONSOLEAPP_LOGGER_HPP
#define CONSOLEAPP_LOGGER_HPP

#include <cstdint>
#include <fstream>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include "tinyfmt.hpp"

class Logger {
  public:
    enum class LogLevel : std::uint8_t {
        kDebug,
        kInfo,
        kWarning,
        kError,
        kFault
    };

    Logger(const Logger &) = delete;
    auto operator=(const Logger &) -> Logger & = delete;

    static auto GetInstance() -> Logger &;

    void SetLevel(LogLevel level);
    void SetRelativeTime(bool relative);
    void SetLogFile(const std::string &filename);

    void Log(LogLevel level, const std::string &message);

    template <typename... Args>
    void LogFormatted(LogLevel level, const std::string &format_string,
                      Args &&...args);

  private:
    struct LogLevelInfo {
        std::string_view text;
        std::string_view color_code;
    };

    static const std::unordered_map<LogLevel, LogLevelInfo> log_level_info_;

    std::ofstream log_file_;
    LogLevel log_level_ = LogLevel::kInfo;
    bool use_relative_time_ = false;
    std::mutex mutex_;
    std::chrono::steady_clock::time_point start_time_;

    Logger();

    static auto GetLevelText(LogLevel level) -> std::string_view;
    static auto GetLevelColor(LogLevel level) -> std::string_view;
    auto GetTimestamp() const -> std::string;
};

template <typename... Args>
void Logger::LogFormatted(LogLevel level, const std::string &format_string,
                          Args &&...args) {
    if (level < log_level_) {
        return;
    }
    if constexpr (sizeof...(Args) == 0) {
        Log(level, format_string);
    } else {
        auto formatted_message =
            tinyfmt::Format(format_string, std::forward<Args>(args)...);
        Log(level, formatted_message);
    }
}

#define LOG_DEBUG(...)                                                         \
    Logger::GetInstance().LogFormatted(Logger::LogLevel::kDebug, __VA_ARGS__)
#define LOG_INFO(...)                                                          \
    Logger::GetInstance().LogFormatted(Logger::LogLevel::kInfo, __VA_ARGS__)
#define LOG_WARNING(...)                                                       \
    Logger::GetInstance().LogFormatted(Logger::LogLevel::kWarning, __VA_ARGS__)
#define LOG_ERROR(...)                                                         \
    Logger::GetInstance().LogFormatted(Logger::LogLevel::kError, __VA_ARGS__)

#define LOG_FAULT(...)                                                         \
    do {                                                                       \
        Logger::GetInstance().LogFormatted(Logger::LogLevel::kFault,           \
                                           __VA_ARGS__);                       \
        throw std::runtime_error(tinyfmt::Format(__VA_ARGS__));                \
    } while (false)
#endif // CONSOLEAPP_LOGGER_HPP
