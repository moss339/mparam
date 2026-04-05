cmake_minimum_required(VERSION 3.15)

project(mparam VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MPARAM_BUILD_TESTS "Build mparam tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CJSON_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../mlog/thirdparty/cJSON")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(MPARAM_SOURCES
    src/param_store.cpp
    src/param_server.cpp
    src/param_client.cpp
    ${CJSON_ROOT}/cJSON.c
)

if(BUILD_SHARED_LIBS)
    add_library(mparam SHARED ${MPARAM_SOURCES})
    set_target_properties(mparam PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
        POSITION_INDEPENDENT_CODE ON
    )
else()
    add_library(mparam STATIC ${MPARAM_SOURCES})
    set_target_properties(mparam PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
    )
endif()

target_include_directories(mparam PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CJSON_ROOT}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(mparam PUBLIC
    Threads::Threads
)

# Export mparam target
export(TARGETS mparam NAMESPACE mparam:: FILE mparamTargets.cmake)

install(TARGETS mparam
    EXPORT mparamTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT mparamTargets
    FILE mparamTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mparam
)

install(DIRECTORY include/mparam/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mparam)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/mparamConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/mparamConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mparam
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/mparamConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mparam
)

if(MPARAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
