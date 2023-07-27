cmake_minimum_required(VERSION 3.5.0)
project(cppgo VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()
add_executable(cppgo cppgo/main.cpp)
add_executable(core_tests tests/core/board.test.cpp tests/core/game.test.cpp)
target_include_directories(cppgo PUBLIC cppgo)
target_include_directories(core_tests PRIVATE cppgo)
target_link_libraries(core_tests GTest::gtest_main)
gtest_discover_tests(core_tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
include(CPack)
