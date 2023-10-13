set(LIB_NAME VirtualMachineLib)

set(SOURCES
    VirtualMachine.cpp
    Interpreter.cpp
    Decoder.cpp
)

add_library(${LIB_NAME} STATIC ${SOURCES})

set(ASM_2_BIN Asm2Bin)
add_executable(${ASM_2_BIN} Parser.cpp Asm2Bin.cpp Decoder.cpp)
