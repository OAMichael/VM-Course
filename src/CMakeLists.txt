file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(LIB_NAME VirtualMachineLib)
set(ASM_2_BIN Asm2Bin)

set(VM_SOURCES
    VirtualMachine.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Decoder.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Interpreter.cpp
)

set(ASM_2_BIN_SOURCES
    Parser.cpp
    Asm2Bin.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Decoder.cpp
)

add_library(${LIB_NAME} STATIC ${VM_SOURCES})
add_executable(${ASM_2_BIN} ${ASM_2_BIN_SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ../include)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(${ASM_2_BIN} PUBLIC ../include)
target_include_directories(${ASM_2_BIN} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(generate_all)

set(TEMPLATES 
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/Instruction_enum.h.temp
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/Executors.h.temp
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/Interpreter.cpp.temp
    ${CMAKE_CURRENT_SOURCE_DIR}/templates/Decoder.cpp.temp
)

foreach(TEMPL ${TEMPLATES})
    get_filename_component(OUTPUT_NAME_TEMPL ${TEMPL} NAME_WLE)
    get_filename_component(TARGET_NAME ${TEMPL} NAME_WE)

    gen_file(
        TEMPLATE ${TEMPL}
        ISA_FILE ${PROJECT_SOURCE_DIR}/isa/isa.yml
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/${OUTPUT_NAME_TEMPL}
    )

    add_custom_target(${TARGET_NAME}_gen DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated/${OUTPUT_NAME_TEMPL})
    add_dependencies(generate_all ${TARGET_NAME}_gen)
endforeach()

add_dependencies(${LIB_NAME} generate_all)
add_dependencies(${ASM_2_BIN} Decoder_gen Instruction_enum_gen)