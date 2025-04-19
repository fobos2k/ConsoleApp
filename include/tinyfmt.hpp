#ifndef CONSOLEAPP_TINYFMT_HPP
#define CONSOLEAPP_TINYFMT_HPP

// tinyfmt.hpp
// Lightweight formatting utility for C++17 (no external dependencies).
// Author: (your name or initials)
// Version: 1.0.0
//
// Usage:
//   tinyfmt::Format("Hello {}, number {}", "world", 42);
//   -> "Hello world, number 42"

#include <cstddef> // for std::nullptr_t
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility> // for std::declval

namespace tinyfmt {

inline constexpr int VersionMajor = 0;
inline constexpr int VersionMinor = 2;
inline constexpr int VersionPatch = 0;

// == Вспомогательная проверка: является ли тип контейнером (имеет begin/end) ==
template <typename T, typename = void> struct is_container : std::false_type {};

template <typename T>
struct is_container<T, std::void_t<decltype(std::declval<T>().begin()),
                                   decltype(std::declval<T>().end())>>
    : std::true_type {};

template <typename T> constexpr bool is_container_v = is_container<T>::value;

// == Подсчёт количества {} в строке ==
inline auto CountPlaceholders(const std::string &format) -> std::size_t {
    std::size_t count = 0;
    std::size_t pos = 0;
    while ((pos = format.find("{}", pos)) != std::string::npos) {
        ++count;
        pos += 2;
    }
    return count;
}

// == Вставка одного аргумента в поток ==

// std::string
inline void InsertArg(std::ostringstream &oss, const std::string &value) {
    oss << value;
}

// std::string_view
inline void InsertArg(std::ostringstream &oss, std::string_view value) {
    oss << value;
}

// bool
inline void InsertArg(std::ostringstream &oss, bool value) {
    oss << (value ? "true" : "false");
}

// nullptr
inline void InsertArg(std::ostringstream &oss, std::nullptr_t) {
    oss << "null";
}

template <typename T>
inline auto InsertArg(std::ostringstream &oss, const T &container)
    -> std::enable_if_t<is_container_v<T>, void> {
    oss << '[';
    bool first = true;
    for (const auto &item : container) {
        if (!first) {
            oss << ", ";
        }
        InsertArg(oss, item);
        first = false;
    }
    oss << ']';
}

template <typename T>
inline auto InsertArg(std::ostringstream &oss, T &&value)
    -> std::enable_if_t<!is_container_v<T>, void> {
    oss << std::forward<T>(value);
}

// == Рекурсивная вставка аргументов ==
inline void FormatRecursive(std::ostringstream &oss,
                            const std::string &format) {
    oss << format;
}

template <typename T, typename... Args>
void FormatRecursive(std::ostringstream &oss, const std::string &format,
                     T &&value, Args &&...args) {
    size_t pos = format.find("{}");
    if (pos == std::string::npos) {
        oss << format;
        return;
    }
    oss << format.substr(0, pos);
    InsertArg(oss, std::forward<T>(value));
    FormatRecursive(oss, format.substr(pos + 2), std::forward<Args>(args)...);
}

// == Публичное API ==
template <typename... Args>
auto Format(const std::string &format, Args &&...args) -> std::string {
    constexpr std::size_t num_args = sizeof...(Args);
    std::size_t num_placeholders = CountPlaceholders(format);

    if (num_args != num_placeholders) {
        throw std::runtime_error("tinyfmt::Format: mismatch between number of "
                                 "placeholders and number of arguments");
    }

    std::ostringstream oss;
    FormatRecursive(oss, format, std::forward<Args>(args)...);
    return oss.str();
}

} // namespace tinyfmt

#endif // CONSOLEAPP_TINYFMT_HPP
