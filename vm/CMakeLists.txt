set(EXEC VirtualMachine)

add_executable(${EXEC} main.cpp)
target_link_libraries(${EXEC} PUBLIC VirtualMachineLib)

if (WIN32)
    add_custom_target(run
        DEPENDS ${EXEC}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}.exe
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "run main"
        VERBATIM
    )
else()
    add_custom_target(run
        DEPENDS ${EXEC}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "run main"
        VERBATIM
    )
endif()