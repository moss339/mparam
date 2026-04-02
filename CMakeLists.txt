cmake_minimum_required(VERSION 3.15)
project(mparam VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MPARAM_BUILD_TESTS "Build mparam tests" ON)

set(CJSON_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../mlog/thirdparty/cJSON")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CJSON_ROOT}
)

set(MPARAM_SOURCES
    src/param_store.cpp
    src/param_server.cpp
    src/param_client.cpp
    ${CJSON_ROOT}/cJSON.c
)

add_library(mparam SHARED ${MPARAM_SOURCES})

target_link_libraries(mparam
    pthread
)

target_include_directories(mparam PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(mparam PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "include/mparam/mparam.h"
)

install(TARGETS mparam
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(MPARAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
