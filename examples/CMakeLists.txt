cmake_minimum_required(VERSION 3.15)
project(mparam_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up mparam library
set(MPARAM_LIBRARY /mnt/data/workspace/moss/mparam/build/libmparam.so)
set(MPARAM_INCLUDE_DIR /mnt/data/workspace/moss/mparam/include)
add_library(mparam SHARED IMPORTED)
set_target_properties(mparam PROPERTIES IMPORTED_LOCATION ${MPARAM_LIBRARY})
set_target_properties(mparam PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${MPARAM_INCLUDE_DIR}")

add_executable(mparam_example
    param_example.cpp
)

target_link_libraries(mparam_example
    mparam
    pthread
)

target_include_directories(mparam_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

install(TARGETS mparam_example DESTINATION bin)
