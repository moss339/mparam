#include "mparam/param_store.h"
#include <algorithm>
#include <ctime>
#include <set>

namespace mparam {

ParameterStore::ParameterStore()
    : next_watcher_id_(1) {
}

ParameterStore::~ParameterStore() = default;

bool ParameterStore::has(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return parameters_.find(key) != parameters_.end();
}

std::optional<Parameter> ParameterStore::get(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = parameters_.find(key);
    if (it != parameters_.end()) {
        return deep_copy(it->second);
    }
    return std::nullopt;
}

Parameter ParameterStore::deep_copy(const Parameter& param) const {
    Parameter copy;
    copy.key = param.key;
    copy.type = param.type;
    copy.version = param.version;
    copy.timestamp = param.timestamp;
    copy.flags = param.flags;

    switch (param.type) {
        case ParamType::STRING:
            copy.value.as_string = new std::string(*param.value.as_string);
            break;
        default:
            copy.value = param.value;
            break;
    }
    return copy;
}

bool ParameterStore::set(const std::string& key, const Parameter& param) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto old_it = parameters_.find(key);
    Parameter old_param;
    bool has_old = false;
    if (old_it != parameters_.end()) {
        old_param = deep_copy(old_it->second);
        has_old = true;
    }

    Parameter new_param = param;
    new_param.version = has_old ? old_param.version + 1 : 1;
    new_param.timestamp = static_cast<int64_t>(std::time(nullptr));

    parameters_[key] = new_param;

    if (has_old) {
        notify_change(key, old_param, new_param);
    }

    return true;
}

bool ParameterStore::set_int(const std::string& key, int64_t value) {
    Parameter param;
    param.key = key;
    param.type = ParamType::INT;
    param.value.as_int = value;
    return set(key, param);
}

bool ParameterStore::set_double(const std::string& key, double value) {
    Parameter param;
    param.key = key;
    param.type = ParamType::DOUBLE;
    param.value.as_double = value;
    return set(key, param);
}

bool ParameterStore::set_bool(const std::string& key, bool value) {
    Parameter param;
    param.key = key;
    param.type = ParamType::BOOL;
    param.value.as_bool = value;
    return set(key, param);
}

bool ParameterStore::set_string(const std::string& key, const std::string& value) {
    Parameter param;
    param.key = key;
    param.type = ParamType::STRING;
    param.value.as_string = new std::string(value);
    return set(key, param);
}

bool ParameterStore::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = parameters_.find(key);
    if (it == parameters_.end()) {
        return false;
    }
    parameters_.erase(it);
    return true;
}

std::vector<Parameter> ParameterStore::get_all() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Parameter> result;
    for (const auto& pair : parameters_) {
        result.push_back(deep_copy(pair.second));
    }
    return result;
}

std::vector<std::string> ParameterStore::get_namespaces() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::set<std::string> namespaces;
    for (const auto& pair : parameters_) {
        size_t pos = pair.first.find('.');
        if (pos != std::string::npos) {
            namespaces.insert(pair.first.substr(0, pos));
        }
    }
    return std::vector<std::string>(namespaces.begin(), namespaces.end());
}

size_t ParameterStore::add_watcher(const std::string& key, ChangeCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    size_t id = next_watcher_id_++;
    watchers_[key].push_back(callback);
    return id;
}

size_t ParameterStore::add_global_watcher(ChangeCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    size_t id = next_watcher_id_++;
    global_watchers_.push_back(callback);
    return id;
}

void ParameterStore::remove_watcher(size_t watcher_id) {
    (void)watcher_id;
}

void ParameterStore::notify_change(const std::string& key,
                                    const Parameter& old_val,
                                    const Parameter& new_val) {
    ParameterChange change;
    change.key = key;
    change.old_value = old_val;
    change.new_value = new_val;
    change.change_time = static_cast<int64_t>(std::time(nullptr));

    auto it = watchers_.find(key);
    if (it != watchers_.end()) {
        for (const auto& callback : it->second) {
            callback(change);
        }
    }

    for (const auto& callback : global_watchers_) {
        callback(change);
    }
}

}  // namespace mparam
