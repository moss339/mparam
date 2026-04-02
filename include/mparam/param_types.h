#ifndef MPARAM_TYPES_H
#define MPARAM_TYPES_H

#include <string>
#include <cstdint>
#include <optional>
#include <variant>
#include <functional>

namespace moss {
namespace mparam {

enum class ParamType : uint8_t {
    NONE = 0,
    INT = 1,
    DOUBLE = 2,
    BOOL = 3,
    STRING = 4,
    JSON = 5
};

union ParamValue {
    int64_t as_int;
    double as_double;
    bool as_bool;
    std::string* as_string;

    ParamValue() : as_int(0) {}
};

struct Parameter {
    std::string key;
    ParamValue value;
    ParamType type;
    int64_t version;
    int64_t timestamp;
    uint32_t flags;

    Parameter() : version(0), timestamp(0), flags(0) {
        value.as_int = 0;
        type = ParamType::NONE;
    }
};

struct ParameterChange {
    std::string key;
    Parameter old_value;
    Parameter new_value;
    int64_t change_time;
};

template<typename T>
struct TypedParameter {
    std::string key;
    T value;
    ParamType type;
    int64_t version;
    bool valid;
};

using ChangeCallback = std::function<void(const ParameterChange&)>;

}  // namespace mparam

}  // namespace moss
#endif  // MPARAM_TYPES_H
