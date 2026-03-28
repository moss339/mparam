#include "mparam/mparam.h"
#include <iostream>

int main() {
    auto server = mparam::ParameterServer::create("/moss_param_demo");
    if (!server->init()) {
        std::cerr << "Failed to initialize parameter server" << std::endl;
        return 1;
    }

    server->declare("vehicle.speed_limit", static_cast<int64_t>(120),
                    "Max speed in km/h");
    server->declare("sensors.lidar.enable", true, "Enable LiDAR sensor");
    server->declare("vehicle.max_acceleration", 3.5, "Max acceleration in m/s^2");
    server->declare("vehicle.name", std::string("Test Vehicle"), "Vehicle name");

    server->set("vehicle.speed_limit", static_cast<int64_t>(100));

    server->watch("vehicle.speed_limit",
        [](const mparam::ParameterChange& change) {
            std::cout << "speed_limit changed" << std::endl;
        });

    auto speed = server->get<int64_t>("vehicle.speed_limit");
    if (speed && speed->valid) {
        std::cout << "Speed limit: " << speed->value << std::endl;
    }

    auto client = mparam::ParameterClient::create("/moss_param_demo");
    if (!client->init()) {
        std::cerr << "Failed to initialize parameter client" << std::endl;
        return 1;
    }

    auto client_speed = client->get<int64_t>("vehicle.speed_limit");
    if (client_speed && client_speed->valid) {
        std::cout << "Client sees speed limit: " << client_speed->value << std::endl;
    }

    client->watch<int64_t>("vehicle.speed_limit",
        [](const std::string& key, int64_t old_val, int64_t new_val) {
            std::cout << key << " changed: " << old_val << " -> " << new_val << std::endl;
        });

    std::cout << "All parameters set and retrieved successfully" << std::endl;
    return 0;
}
