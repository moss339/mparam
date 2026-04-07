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

    mparam::ParamTraits<std::string>::set(value, "hello");
    assert(mparam::ParamTraits<std::string>::get(value) == "hello");

    std::cout << "test_param_traits passed" << std::endl;
}

void test_parameter_store_basic() {
    mparam::ParameterStore store;

    mparam::Parameter param;
    param.key = "test_int";
    param.type = mparam::ParamType::INT;
    param.value = int64_t(100);
    param.version = 1;

    bool result = store.set("test_int", param);
    assert(result);
    assert(store.has("test_int"));

    auto retrieved = store.get("test_int");
    assert(retrieved != std::nullopt);
    assert(std::get<int64_t>(retrieved->value) == 100);

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
    assert(std::get<std::string>(param->value) == "TestVehicle");

    std::cout << "test_parameter_store_string passed" << std::endl;
}

void test_parameter_store_version() {
    mparam::ParameterStore store;

    mparam::Parameter param;
    param.key = "version_test";
    param.type = mparam::ParamType::INT;
    param.value = int64_t(1);
    param.version = 0;

    store.set("version_test", param);
    auto first = store.get("version_test");
    assert(first != std::nullopt);
    assert(first->version == 1);

    param.value = int64_t(2);
    store.set("version_test", param);
    auto second = store.get("version_test");
    assert(second != std::nullopt);
    assert(second->version == 2);

    std::cout << "test_parameter_store_version passed" << std::endl;
}

void test_parameter_store_namespaces() {
    mparam::ParameterStore store;

    store.set_int("control.max_speed", 100);
    store.set_int("control.min_speed", 0);
    store.set_double("planning.lookahead", 50.0);

    auto namespaces = store.get_namespaces();
    assert(namespaces.size() >= 2);

    std::cout << "test_parameter_store_namespaces passed" << std::endl;
}

void test_parameter_store_remove() {
    mparam::ParameterStore store;

    store.set_int("temp", 999);
    assert(store.has("temp"));

    bool removed = store.remove("temp");
    assert(removed);
    assert(!store.has("temp"));

    std::cout << "test_parameter_store_remove passed" << std::endl;
}

void test_parameter_server_create() {
    auto server = mparam::ParameterServer::create("/tmp/test_param_shm");
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
