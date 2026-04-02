#include "mparam/param_server.h"
#include <fstream>
#include <sstream>
#include <cJSON.h>

namespace moss {
namespace mparam {

std::shared_ptr<ParameterServer> ParameterServer::create(const std::string& shm_name) {
    return std::make_shared<ParameterServer>(shm_name);
}

ParameterServer::ParameterServer(const std::string& shm_name)
    : shm_name_(shm_name), initialized_(false) {
}

ParameterServer::~ParameterServer() {
    shutdown();
}

bool ParameterServer::init() {
    if (initialized_) {
        return true;
    }
    initialized_ = true;
    return true;
}

void ParameterServer::shutdown() {
    initialized_ = false;
}

bool ParameterServer::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();

    cJSON* root = cJSON_Parse(content.c_str());
    if (!root) {
        return false;
    }

    cJSON* params = cJSON_GetObjectItem(root, "parameters");
    if (!params || !cJSON_IsArray(params)) {
        cJSON_Delete(root);
        return false;
    }

    int size = cJSON_GetArraySize(params);
    for (int i = 0; i < size; ++i) {
        cJSON* param = cJSON_GetArrayItem(params, i);
        if (!param) continue;

        cJSON* key = cJSON_GetObjectItem(param, "key");
        cJSON* type = cJSON_GetObjectItem(param, "type");
        if (!key || !type || !cJSON_IsString(key) || !cJSON_IsString(type)) {
            continue;
        }

        std::string key_str = key->valuestring;
        std::string type_str = type->valuestring;

        if (type_str == "int") {
            cJSON* value = cJSON_GetObjectItem(param, "value");
            if (value && cJSON_IsNumber(value)) {
                store_.set_int(key_str, static_cast<int64_t>(value->valueint));
            }
        } else if (type_str == "double") {
            cJSON* value = cJSON_GetObjectItem(param, "value");
            if (value && cJSON_IsNumber(value)) {
                store_.set_double(key_str, value->valuedouble);
            }
        } else if (type_str == "bool") {
            cJSON* value = cJSON_GetObjectItem(param, "value");
            if (value && cJSON_IsBool(value)) {
                store_.set_bool(key_str, cJSON_IsTrue(value));
            }
        } else if (type_str == "string") {
            cJSON* value = cJSON_GetObjectItem(param, "value");
            if (value && cJSON_IsString(value)) {
                store_.set_string(key_str, value->valuestring);
            }
        }
    }

    cJSON_Delete(root);
    return true;
}

bool ParameterServer::save_to_file(const std::string& path) const {
    (void)path;
    return false;
}

ParameterStore& ParameterServer::get_store() {
    return store_;
}

}  // namespace mparam

}  // namespace moss
