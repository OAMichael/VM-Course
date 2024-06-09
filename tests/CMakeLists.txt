add_custom_target(run_all_tests)

set(ASM_FILES
    ${PROJECT_SOURCE_DIR}/asm/tests/arithmetic.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/array.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/branching.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/call.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/intrinsic.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/square_eq.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/strfunc.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/string.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/recursion.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/object.asm
    ${PROJECT_SOURCE_DIR}/asm/tests/objarray.asm
)

add_custom_target(generate_asm_tests)

foreach(SOURCE_ASM ${ASM_FILES})
    get_filename_component(TARGET_NAME ${SOURCE_ASM} NAME_WE)
    add_custom_target(generate_asm_${TARGET_NAME}
        COMMENT "Generate binary for ${TARGET_NAME}"
        COMMAND ${CMAKE_BINARY_DIR}/src/Asm2Bin$<IF:${WIN32},.exe,> ${SOURCE_ASM}
    )
    add_dependencies(generate_asm_${TARGET_NAME} Asm2Bin)
    add_dependencies(generate_asm_tests generate_asm_${TARGET_NAME})
endforeach()

set(TESTS
    tests
)

foreach(EXEC IN LISTS TESTS)
    add_executable(${EXEC} ${EXEC}.cpp)
    target_link_libraries(${EXEC} VirtualMachineLib GTest::gtest_main)

    add_custom_target(run_test_${EXEC}
        DEPENDS ${EXEC} generate_asm_tests
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}$<IF:${WIN32},.exe,>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "run test ${EXEC}"
        VERBATIM
    )
    add_dependencies(run_all_tests run_test_${EXEC})
endforeach()