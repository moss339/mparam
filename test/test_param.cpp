#include "mparam/mparam.h"
#include <iostream>
#include <cassert>
#include <string>

void test_param_types() {
    assert(mparam::ParamType::NONE == mparam::ParamType(0));
    assert(mparam::ParamType::INT == mparam::ParamType(1));
    assert(mparam::ParamType::DOUBLE == mparam::ParamType(2));
    assert(mparam::ParamType::BOOL == mparam::ParamType(3));
    assert(mparam::ParamType::STRING == mparam::ParamType(4));
    assert(mparam::ParamType::JSON == mparam::ParamType(5));
    std::cout << "test_param_types passed" << std::endl;
}

void test_param_traits() {
    assert(mparam::ParamTraits<int64_t>::type == mparam::ParamType::INT);
    assert(mparam::ParamTraits<double>::type == mparam::ParamType::DOUBLE);
    assert(mparam::ParamTraits<bool>::type == mparam::ParamType::BOOL);
    assert(mparam::ParamTraits<std::string>::type == mparam::ParamType::STRING);

    mparam::ParamValue value;
    mparam::ParamTraits<int64_t>::set(value, 42);
    assert(mparam::ParamTraits<int64_t>::get(value) == 42);

    mparam::ParamTraits<double>::set(value, 3.14);
    assert(mparam::ParamTraits<double>::get(value) == 3.14);

    mparam::ParamTraits<bool>::set(value, true);
    assert(mparam::ParamTraits<bool>::get(value) == true);

    std::cout << "test_param_traits passed" << std::endl;
}

void test_parameter_store_basic() {
    mparam::ParameterStore store;

    assert(!store.has("test"));
    assert(store.get("test") == std::nullopt);

    mparam::Parameter param;
    param.key = "test_int";
    param.type = mparam::ParamType::INT;
    param.value.as_int = 100;
    param.version = 1;

    bool result = store.set("test_int", param);
    assert(result);
    assert(store.has("test_int"));

    auto retrieved = store.get("test_int");
    assert(retrieved != std::nullopt);
    assert(retrieved->value.as_int == 100);

    std::cout << "test_parameter_store_basic passed" << std::endl;
}

void test_parameter_store_int_doubles() {
    mparam::ParameterStore store;

    store.set_int("int_val", 42);
    store.set_double("double_val", 3.14159);
    store.set_bool("bool_val", true);

    auto int_param = store.get("int_val");
    assert(int_param != std::nullopt);
    assert(int_param->type == mparam::ParamType::INT);

    auto double_param = store.get("double_val");
    assert(double_param != std::nullopt);
    assert(double_param->type == mparam::ParamType::DOUBLE);

    auto bool_param = store.get("bool_val");
    assert(bool_param != std::nullopt);
    assert(bool_param->type == mparam::ParamType::BOOL);

    std::cout << "test_parameter_store_int_doubles passed" << std::endl;
}

void test_parameter_store_string() {
    mparam::ParameterStore store;

    store.set_string("name", "TestVehicle");

    auto param = store.get("name");
    assert(param != std::nullopt);
    assert(param->type == mparam::ParamType::STRING);
    assert(*param->value.as_string == "TestVehicle");

    std::cout << "test_parameter_store_string passed" << std::endl;
}

void test_parameter_store_version() {
    mparam::ParameterStore store;

    mparam::Parameter param;
    param.key = "version_test";
    param.type = mparam::ParamType::INT;
    param.value.as_int = 1;
    param.version = 1;

    store.set("version_test", param);

    param.value.as_int = 2;
    store.set("version_test", param);

    auto retrieved = store.get("version_test");
    assert(retrieved != std::nullopt);
    assert(retrieved->version == 2);
    assert(retrieved->value.as_int == 2);

    std::cout << "test_parameter_store_version passed" << std::endl;
}

void test_parameter_store_namespaces() {
    mparam::ParameterStore store;

    store.set_int("vehicle.speed_limit", 120);
    store.set_int("vehicle.max_acceleration", 5);
    store.set_bool("sensors.lidar.enable", true);

    auto namespaces = store.get_namespaces();
    assert(namespaces.size() == 2);

    auto all_params = store.get_all();
    assert(all_params.size() == 3);

    std::cout << "test_parameter_store_namespaces passed" << std::endl;
}

void test_parameter_store_remove() {
    mparam::ParameterStore store;

    store.set_int("to_remove", 100);
    assert(store.has("to_remove"));

    bool result = store.remove("to_remove");
    assert(result);
    assert(!store.has("to_remove"));

    result = store.remove("non_existent");
    assert(!result);

    std::cout << "test_parameter_store_remove passed" << std::endl;
}

void test_parameter_server_create() {
    auto server = mparam::ParameterServer::create("/test_param");
    assert(server != nullptr);

    bool result = server->init();
    assert(result);

    std::cout << "test_parameter_server_create passed" << std::endl;
}

void test_typed_parameter() {
    mparam::TypedParameter<int64_t> tp;
    tp.key = "test";
    tp.value = 42;
    tp.type = mparam::ParamType::INT;
    tp.version = 1;
    tp.valid = true;

    assert(tp.key == "test");
    assert(tp.value == 42);
    assert(tp.valid == true);

    std::cout << "test_typed_parameter passed" << std::endl;
}

int main() {
    test_param_types();
    test_param_traits();
    test_parameter_store_basic();
    test_parameter_store_int_doubles();
    test_parameter_store_string();
    test_parameter_store_version();
    test_parameter_store_namespaces();
    test_parameter_store_remove();
    test_parameter_server_create();
    test_typed_parameter();

    std::cout << "All parameter tests passed!" << std::endl;
    return 0;
}
