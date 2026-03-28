#ifndef MPARAM_PARAM_SERVER_H
#define MPARAM_PARAM_SERVER_H

#include "param_traits.h"
#include "param_store.h"
#include <memory>
#include <string>

namespace mparam {

class ParameterServer : public std::enable_shared_from_this<ParameterServer> {
public:
    static std::shared_ptr<ParameterServer> create(
        const std::string& shm_name = "/moss_param");

    explicit ParameterServer(const std::string& shm_name = "/moss_param");
    ~ParameterServer();

    bool init();
    void shutdown();

    template<typename T>
    std::optional<TypedParameter<T>> get(const std::string& key);

    template<typename T>
    bool set(const std::string& key, const T& value);

    template<typename T>
    bool declare(const std::string& key, const T& default_value,
                 const std::string& description = "");

    size_t watch(const std::string& key,
                 std::function<void(const ParameterChange&)> callback);

    bool load_from_file(const std::string& path);
    bool save_to_file(const std::string& path) const;

    ParameterStore& get_store();

private:
    std::string shm_name_;
    ParameterStore store_;
    bool initialized_;
};

template<typename T>
std::optional<TypedParameter<T>> ParameterServer::get(const std::string& key) {
    auto param = store_.get(key);
    if (!param) return std::nullopt;

    TypedParameter<T> result;
    result.key = key;
    result.version = param->version;
    result.valid = (param->type == ParamTraits<T>::type);

    if (result.valid) {
        result.value = ParamTraits<T>::get(param->value);
    }

    return result;
}

template<typename T>
bool ParameterServer::set(const std::string& key, const T& value) {
    Parameter param;
    param.key = key;
    param.type = ParamTraits<T>::type;
    param.version = 1;
    param.timestamp = 0;
    ParamTraits<T>::set(param.value, value);

    return store_.set(key, param);
}

template<typename T>
bool ParameterServer::declare(const std::string& key, const T& default_value,
                               const std::string& description) {
    (void)description;
    return set<T>(key, default_value);
}

}  // namespace mparam

#endif  // MPARAM_PARAM_SERVER_H
