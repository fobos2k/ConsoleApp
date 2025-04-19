#ifndef CONSOLEAPP_TIME_UTILS_HPP
#define CONSOLEAPP_TIME_UTILS_HPP

#include <string>

namespace common {
namespace time {

auto CurrentDateTime() -> std::string;
auto SinceAppStart() -> std::string;

}; // namespace time
}; // namespace common

#endif // CONSOLEAPP_TIME_UTILS_HPP
