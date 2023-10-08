set(EXEC tests)
add_executable(${EXEC} test1.cpp)
target_link_libraries(${EXEC} VirtualMachineLib GTest::gtest_main)

if (WIN32)
    add_custom_target(run_tests
        DEPENDS ${EXEC}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}.exe
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "run tests"
        VERBATIM
    )
else()
    add_custom_target(run_tests
        DEPENDS ${EXEC}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "run tests"
        VERBATIM
    )
endif()