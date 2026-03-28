set(MPARAM_TEST_SOURCES
    test_param.cpp
)

add_executable(mparam_tests ${MPARAM_TEST_SOURCES})

target_link_libraries(mparam_tests
    mparam
    pthread
)

add_test(NAME mparam_tests COMMAND mparam_tests)

install(TARGETS mparam_tests DESTINATION bin)
