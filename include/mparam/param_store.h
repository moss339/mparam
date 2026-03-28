#ifndef MPARAM_PARAM_STORE_H
#define MPARAM_PARAM_STORE_H

#include "param_types.h"
#include <map>
#include <vector>
#include <mutex>
#include <functional>

namespace mparam {

class ParameterStore {
public:
    ParameterStore();
    ~ParameterStore();

    bool has(const std::string& key) const;
    std::optional<Parameter> get(const std::string& key) const;

    bool set(const std::string& key, const Parameter& param);
    bool set_int(const std::string& key, int64_t value);
    bool set_double(const std::string& key, double value);
    bool set_bool(const std::string& key, bool value);
    bool set_string(const std::string& key, const std::string& value);

    bool remove(const std::string& key);

    std::vector<Parameter> get_all() const;
    std::vector<std::string> get_namespaces() const;

    size_t add_watcher(const std::string& key, ChangeCallback callback);
    size_t add_global_watcher(ChangeCallback callback);
    void remove_watcher(size_t watcher_id);

    void notify_change(const std::string& key, const Parameter& old_val,
                       const Parameter& new_val);

private:
    Parameter deep_copy(const Parameter& param) const;

    std::map<std::string, Parameter> parameters_;
    std::map<std::string, std::vector<ChangeCallback>> watchers_;
    std::vector<ChangeCallback> global_watchers_;
    size_t next_watcher_id_;
    mutable std::mutex mutex_;
};

}  // namespace mparam

#endif  // MPARAM_PARAM_STORE_H
