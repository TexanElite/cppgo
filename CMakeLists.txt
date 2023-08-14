cmake_minimum_required(VERSION 3.5.0)
project(cppgo VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1
)

FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost
  GIT_TAG        b6928ae5c92e21a04bbe17a558e6e066dbe632f6
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Boost googletest)

find_package(OpenSSL REQUIRED)
include(GoogleTest)

enable_testing()
add_executable(cppgo cppgo/main.cpp)
add_executable(ogs_cppgo cppgo/ogs/ogs_cppgo.cpp)
add_executable(core_tests tests/core/board.test.cpp tests/core/game.test.cpp)
target_include_directories(cppgo PRIVATE cppgo)
target_include_directories(ogs_cppgo PRIVATE cppgo)
target_include_directories(core_tests PRIVATE cppgo)
target_link_libraries(core_tests PRIVATE GTest::gtest_main)
target_link_libraries(ogs_cppgo PRIVATE Boost::json Boost::program_options OpenSSL::SSL OpenSSL::Crypto)
gtest_discover_tests(core_tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
include(CPack)
