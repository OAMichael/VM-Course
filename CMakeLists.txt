cmake_minimum_required(VERSION 3.13)

project(VirtualMachine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include(./cmake/generation.cmake)

add_subdirectory(thirdParty/googletest)

include_directories(include)
add_subdirectory(src)
add_subdirectory(vm)
add_subdirectory(tests)