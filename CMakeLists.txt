cmake_minimum_required(VERSION 3.10)

project(VirtualMachine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(COMPILE_OPT -std=c++20 -O2)
set(SRC_DIR ./src)
set(INCLUDE_DIR ./include)


add_executable(VirtualMachine
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/VirtualMachine.cpp
    ${SRC_DIR}/Interpreter.cpp
    ${SRC_DIR}/Decoder.cpp
)

target_include_directories(VirtualMachine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${INCLUDE_DIR}
)

target_link_libraries(VirtualMachine PUBLIC ${LINK_OPT})
add_compile_options(VirtualMachine ${COMPILE_OPT})
