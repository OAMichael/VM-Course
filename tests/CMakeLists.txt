add_custom_target(run_all_tests)

set(TESTS
    tests
)

foreach(EXEC IN LISTS TESTS)
    add_executable(${EXEC} ${EXEC}.cpp)
    target_link_libraries(${EXEC} VirtualMachineLib GTest::gtest_main)
    if (WIN32)
        add_custom_target(run_test_${EXEC}
            DEPENDS ${EXEC}
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}.exe
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "run test ${EXEC}"
            VERBATIM
        )
    else()
        add_custom_target(run_test_${EXEC}
            DEPENDS ${EXEC}
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "run test ${EXEC}"
            VERBATIM
        )
    endif()
    add_dependencies(run_all_tests run_test_${EXEC})
endforeach()