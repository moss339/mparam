#ifndef MPARAM_PARAM_TRAITS_H
#define MPARAM_PARAM_TRAITS_H

#include "param_types.h"
#include <type_traits>
#include <stdexcept>

namespace moss {
namespace mparam {

template<typename T>
struct ParamTraits;

template<>
struct ParamTraits<int64_t> {
    static constexpr ParamType type = ParamType::INT;
    static int64_t get(const ParamValue& value) {
        return std::get<int64_t>(value);
    }
    static void set(ParamValue& value, int64_t v) {
        value = v;
    }
};

template<>
struct ParamTraits<double> {
    static constexpr ParamType type = ParamType::DOUBLE;
    static double get(const ParamValue& value) {
        return std::get<double>(value);
    }
    static void set(ParamValue& value, double v) {
        value = v;
    }
};

template<>
struct ParamTraits<bool> {
    static constexpr ParamType type = ParamType::BOOL;
    static bool get(const ParamValue& value) {
        return std::get<bool>(value);
    }
    static void set(ParamValue& value, bool v) {
        value = v;
    }
};

template<>
struct ParamTraits<std::string> {
    static constexpr ParamType type = ParamType::STRING;
    static std::string get(const ParamValue& value) {
        return std::get<std::string>(value);
    }
    static void set(ParamValue& value, const std::string& v) {
        value = v;
    }
};

// Helper function to get value safely
template<typename T>
std::optional<T> get_value(const ParamValue& value) {
    try {
        return std::get<T>(value);
    } catch (const std::bad_variant_access&) {
        return std::nullopt;
    }
}

// Helper to check if value holds a specific type
template<typename T>
bool holds_type(const ParamValue& value) {
    return std::holds_alternative<T>(value);
}

}  // namespace mparam
}  // namespace moss
#endif  // MPARAM_PARAM_TRAITS_H
