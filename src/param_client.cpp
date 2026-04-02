#include "mparam/param_client.h"

namespace moss {
namespace mparam {

std::shared_ptr<ParameterClient> ParameterClient::create(const std::string& shm_name) {
    return std::make_shared<ParameterClient>(shm_name);
}

ParameterClient::ParameterClient(const std::string& shm_name)
    : shm_name_(shm_name),
      local_store_(std::make_shared<ParameterStore>()),
      connected_(false) {
}

ParameterClient::~ParameterClient() {
}

bool ParameterClient::init() {
    connected_ = true;
    return true;
}

bool ParameterClient::is_connected() const {
    return connected_;
}

}  // namespace mparam

}  // namespace moss
