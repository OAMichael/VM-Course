set(EXEC VirtualMachine)

add_executable(${EXEC} main.cpp)
target_link_libraries(${EXEC} PUBLIC VirtualMachineLib)

add_custom_target(run
    DEPENDS ${EXEC} generate_asm_main
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}$<IF:${WIN32},.exe,>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "run main"
    VERBATIM
)