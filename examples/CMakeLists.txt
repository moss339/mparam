add_executable(mparam_example
    param_example.cpp
)

target_link_libraries(mparam_example
    mparam
    pthread
)

target_include_directories(mparam_example PRIVATE
    ${CMAKE_SOURCE_DIR}/mparam/include
)

install(TARGETS mparam_example DESTINATION bin)
