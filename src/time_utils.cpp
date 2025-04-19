#include "time_utils.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

static std::chrono::steady_clock::time_point app_start_time_ =
    std::chrono::steady_clock::now();

constexpr auto kMicrosecondsPerSecond =
    std::chrono::seconds(1) / std::chrono::microseconds(1);
constexpr int kTimestampPrecision = 6;

void SetAppStartTime(std::chrono::steady_clock::time_point point) {
    app_start_time_ = point;
}

auto common::time::CurrentDateTime() -> std::string {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time_t_now = system_clock::to_time_t(now);
    auto usec = duration_cast<microseconds>(now.time_since_epoch()) %
                kMicrosecondsPerSecond;

    std::tm tm_now{};
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setw(kTimestampPrecision) << std::setfill('0')
        << usec.count();

    return oss.str();
}

auto common::time::SinceAppStart() -> std::string {
    using namespace std::chrono;
    auto now = steady_clock::now();
    auto elapsed_us = duration_cast<microseconds>(now - app_start_time_);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(kTimestampPrecision)
        << static_cast<double>(elapsed_us.count()) / kMicrosecondsPerSecond
        << "s";
    return oss.str();
}
