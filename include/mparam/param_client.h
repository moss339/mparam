#ifndef MPARAM_PARAM_CLIENT_H
#define MPARAM_PARAM_CLIENT_H

#include "param_store.h"
#include "param_traits.h"
#include <memory>
#include <string>
#include <future>

namespace mparam {

class ParameterClient : public std::enable_shared_from_this<ParameterClient> {
public:
    static std::shared_ptr<ParameterClient> create(
        const std::string& shm_name = "/moss_param");

    explicit ParameterClient(const std::string& shm_name = "/moss_param");
    ~ParameterClient();

    bool init();
    bool is_connected() const;

    template<typename T>
    std::optional<TypedParameter<T>> get(const std::string& key);

    template<typename T>
    bool set(const std::string& key, const T& value);

    template<typename T>
    std::future<std::optional<TypedParameter<T>>> async_get(
        const std::string& key);

    template<typename T>
    size_t watch(const std::string& key,
                 std::function<void(const std::string&, T, T)> callback);

private:
    std::string shm_name_;
    std::shared_ptr<ParameterStore> local_store_;
    bool connected_;
};

template<typename T>
std::optional<TypedParameter<T>> ParameterClient::get(const std::string& key) {
    auto param = local_store_->get(key);
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
bool ParameterClient::set(const std::string& key, const T& value) {
    Parameter param;
    param.key = key;
    param.type = ParamTraits<T>::type;
    param.version = 1;
    param.timestamp = 0;
    ParamTraits<T>::set(param.value, value);

    return local_store_->set(key, param);
}

template<typename T>
std::future<std::optional<TypedParameter<T>>> ParameterClient::async_get(
    const std::string& key) {
    return std::async(std::launch::async, [this, key]() {
        return get<T>(key);
    });
}

template<typename T>
size_t ParameterClient::watch(const std::string& key,
                              std::function<void(const std::string&, T, T)> callback) {
    return local_store_->add_watcher(key,
        [callback](const ParameterChange& change) {
            if (change.old_value.type == ParamTraits<T>::type &&
                change.new_value.type == ParamTraits<T>::type) {
                callback(change.key,
                        ParamTraits<T>::get(change.old_value.value),
                        ParamTraits<T>::get(change.new_value.value));
            }
        });
}

}  // namespace mparam

#endif  // MPARAM_PARAM_CLIENT_H
