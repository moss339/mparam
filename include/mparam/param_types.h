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

// B18 fix: Use std::variant instead of unsafe union
using ParamValue = std::variant<
    std::monostate,  // NONE
    int64_t,         // INT
    double,          // DOUBLE
    bool,            // BOOL
    std::string      // STRING
>;

struct Parameter {
    std::string key;
    ParamValue value;
    ParamType type;
    int64_t version;
    int64_t timestamp;
    uint32_t flags;

    Parameter() : value(std::monostate{}), type(ParamType::NONE),
                  version(0), timestamp(0), flags(0) {}
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

// Convenience namespace alias
namespace mparam = moss::mparam;

#endif  // MPARAM_TYPES_H
