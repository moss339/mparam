#ifndef MPARAM_PARAM_TRAITS_H
#define MPARAM_PARAM_TRAITS_H

#include "param_types.h"
#include <type_traits>

namespace mparam {

template<typename T>
struct ParamTraits;

template<>
struct ParamTraits<int64_t> {
    static constexpr ParamType type = ParamType::INT;
    static int64_t get(const ParamValue& value) { return value.as_int; }
    static void set(ParamValue& value, int64_t v) { value.as_int = v; }
};

template<>
struct ParamTraits<double> {
    static constexpr ParamType type = ParamType::DOUBLE;
    static double get(const ParamValue& value) { return value.as_double; }
    static void set(ParamValue& value, double v) { value.as_double = v; }
};

template<>
struct ParamTraits<bool> {
    static constexpr ParamType type = ParamType::BOOL;
    static bool get(const ParamValue& value) { return value.as_bool; }
    static void set(ParamValue& value, bool v) { value.as_bool = v; }
};

template<>
struct ParamTraits<std::string> {
    static constexpr ParamType type = ParamType::STRING;
    static std::string get(const ParamValue& value) { return *value.as_string; }
    static void set(ParamValue& value, const std::string& v) {
        if (!value.as_string) {
            value.as_string = new std::string();
        }
        *value.as_string = v;
    }
};

}  // namespace mparam

#endif  // MPARAM_PARAM_TRAITS_H
