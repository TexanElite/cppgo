cmake_minimum_required(VERSION 3.5.0)
project(cppgo VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(cppgo cppgo/main.cpp)
target_include_directories(cppgo PUBLIC cppgo)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
include(CPack)
