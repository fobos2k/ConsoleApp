#ifndef CONSOLEAPP_LOGGER_HPP
#define CONSOLEAPP_LOGGER_HPP

#include <cstdint>
#include <fstream>
#include <mutex>

#include "tinyfmt.hpp"

enum class LogLevel : std::uint8_t { kDebug, kInfo, kWarning, kError, kFault };

class Logger {
  public:
    Logger(const Logger &) = delete;
    auto operator=(const Logger &) = delete;

    static auto GetInstance() -> Logger & {
        static Logger instance;
        return instance;
    }

    void SetLevel(LogLevel level) {
        log_level_ = level;
    }
    void SetRelativeTime(bool relative) {
        use_relative_time_ = relative;
    }
    void SetLogFile(const std::string &filename);
    void Log(LogLevel level, const std::string &message);

    template <typename... Args>
    void LogFormatted(LogLevel level, const std::string &fmt_str,
                      Args &&...args) {
        if constexpr (sizeof...(Args) == 0) {
            Log(level, fmt_str);
        } else {
            auto formatted =
                tinyfmt::Format(fmt_str, std::forward<Args>(args)...);
            Log(level, formatted);
        }
    }

  private:
    std::ofstream log_file_;
    LogLevel log_level_ = LogLevel::kInfo;
    bool use_relative_time_ = false;
    std::mutex mutex_;
    std::chrono::steady_clock::time_point start_time;

    Logger() : start_time(std::chrono::steady_clock::now()) {}

    static auto LevelToString(LogLevel level) -> std::string;
    auto GetTimeStamp() const -> std::string;
};

#define LOG_DEBUG(...)                                                         \
    Logger::GetInstance().LogFormatted(LogLevel::kDebug, __VA_ARGS__)
#define LOG_INFO(...)                                                          \
    Logger::GetInstance().LogFormatted(LogLevel::kInfo, __VA_ARGS__)
#define LOG_WARNING(...)                                                       \
    Logger::GetInstance().LogFormatted(LogLevel::kWarning, __VA_ARGS__)
#define LOG_ERROR(...)                                                         \
    Logger::GetInstance().LogFormatted(LogLevel::kError, __VA_ARGS__)

#define LOG_FAULT(...)                                                         \
    do {                                                                       \
        Logger::GetInstance().LogFormatted(LogLevel::kFault, __VA_ARGS__);     \
        throw std::runtime_error(tinyfmt::Format(__VA_ARGS__));                \
    } while (false)
#endif // CONSOLEAPP_LOGGER_HPP
