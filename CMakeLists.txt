cmake_minimum_required(VERSION 3.13)

project(VirtualMachine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPILE_OPTIONS
	-Wall
	-Werror
	-Wextra
	-Wconversion
	-Wwrite-strings
	-Wcast-qual
	-Wundef
	-Wlogical-op
)

if(NOT WIN32)
    set(WIN32 0)
endif()

include(./cmake/codeGeneration.cmake)

set(gtest_force_shared_crt on)
add_subdirectory(thirdParty/googletest)

add_compile_options(${COMPILE_OPTIONS})

add_subdirectory(src)
add_subdirectory(vm)
add_subdirectory(frontend)
add_subdirectory(tests)