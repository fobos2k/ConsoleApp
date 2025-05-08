#ifndef CONSOLEAPP_TINYFMT_HPP
#define CONSOLEAPP_TINYFMT_HPP

#include <cstddef>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "resources/tinyfmt.hpp"

namespace tinyfmt {

inline constexpr int VersionMajor = 0;
inline constexpr int VersionMinor = 3;
inline constexpr int VersionPatch = 0;

//===================
// Type Traits
//===================

// Check if T is a container (currently only std::vector)
template <typename T> struct is_container : std::false_type {};

template <typename T, typename Alloc>
struct is_container<std::vector<T, Alloc>> : std::true_type {};

template <typename T>
constexpr bool is_container_v = is_container<std::decay_t<T>>::value;

// Check if T is a tuple
template <typename T> struct is_tuple : std::false_type {};

template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

template <typename T>
constexpr bool is_tuple_v = is_tuple<std::decay_t<T>>::value;

// Check if T is a std::optional
template <typename T> struct is_optional : std::false_type {};

template <typename T> struct is_optional<std::optional<T>> : std::true_type {};

template <typename T>
constexpr bool is_optional_v = is_optional<std::decay_t<T>>::value;

//===================
// Helpers
//===================

// Insert a single tuple element into the output stream
template <typename T>
void InsertTupleElement(std::ostringstream &oss, T &&value) {
    if constexpr (std::is_same_v<std::decay_t<T>, std::string> ||
                  std::is_same_v<std::decay_t<T>, std::string_view>) {
        oss << '"' << value << '"'; // Strings are wrapped in quotes
    } else if constexpr (is_optional_v<std::decay_t<T>>) {
        if (value.has_value()) {
            InsertTupleElement(oss, *value); // Unwrap optional
        } else {
            oss << "null";
        }
    } else {
        oss << value;
    }
}

//===================
// Core Formatting
//===================

// Base: Insert scalar types
template <typename T>
void InsertArg(std::ostringstream &oss, T &&value)
    requires(!is_container_v<std::decay_t<T>>)
{
    if constexpr (is_optional_v<std::decay_t<T>>) {
        if (value.has_value()) {
            InsertArg(oss, *value);
        } else {
            oss << "null";
        }
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::string> ||
                         std::is_same_v<std::decay_t<T>, std::string_view>) {
        oss << '"' << value << '"';
    } else {
        oss << std::forward<T>(value);
    }
}

// Then: Insert container of non-tuples
template <typename T>
void InsertArg(std::ostringstream &oss, const T &container)
    requires is_container_v<T> && (!is_tuple_v<typename T::value_type>)
{
    oss << "[\n";
    bool first = true;
    for (const auto &item : container) {
        if (!first) {
            oss << ",\n";
        }
        oss << "  ";
        InsertArg(oss, item);
        first = false;
    }
    oss << "\n]";
}

// Then: Insert container of tuples
template <typename T>
void InsertArg(std::ostringstream &oss, const T &container)
    requires is_container_v<T> && is_tuple_v<typename T::value_type>
{
    oss << "[\n";
    bool first = true;
    for (const auto &tuple : container) {
        if (!first) {
            oss << ",\n";
        }
        oss << "  (";

        std::apply(
            [&oss](auto &&...elems) {
                bool inner_first = true;
                ((oss << (inner_first ? "" : ", "),
                  InsertTupleElement(oss, elems), inner_first = false),
                 ...);
            },
            tuple);

        oss << ')';
        first = false;
    }
    oss << "\n]";
}

//===================
// Format Core Logic
//===================

// Base case: no more arguments to insert
inline void FormatRecursive(std::ostringstream &oss,
                            const std::string &format) {
    oss << format;
}

// Recursive case: insert one argument and continue
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

//===================
// Public API
//===================

// Count the number of "{}" placeholders in the format string
inline auto CountPlaceholders(const std::string &format) -> std::size_t {
    std::size_t count = 0;
    std::size_t pos = 0;
    while ((pos = format.find("{}", pos)) != std::string::npos) {
        ++count;
        pos += 2;
    }
    return count;
}

// Format function: main user entry point
template <typename... Args>
auto Format(const std::string &format, Args &&...args) -> std::string {
    constexpr std::size_t num_args = sizeof...(Args);
    std::size_t num_placeholders = CountPlaceholders(format);

    if (num_args != num_placeholders) {
        throw std::runtime_error(
            resources::tinyfmt::kTinyFmtMismatchError.data());
    }

    std::ostringstream oss;
    FormatRecursive(oss, format, std::forward<Args>(args)...);
    return oss.str();
}

} // namespace tinyfmt

#endif // CONSOLEAPP_TINYFMT_HPP
