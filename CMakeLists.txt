cmake_minimum_required(VERSION 3.0.0)
project(hackerrank VERSION 0.1.0)

set(ALL_CLANG_WARNINGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wimplicit-fallthrough")
set(ALL_GCC_WARNINGS "${ALL_CLANG_WARNINGS} -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wuseless-cast")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${ALL_CLANG_WARNINGS} -Werror")
else()
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${ALL_GCC_WARNINGS} -Werror")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hackerrank main2.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
