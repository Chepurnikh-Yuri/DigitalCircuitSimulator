cmake_minimum_required(VERSION 3.10)

project(Models LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(Models INTERFACE)
target_include_directories(Models INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# gtest
add_subdirectory(third_party/googletest)

# test
enable_testing()
add_executable(ModelsTest test/test.cpp)
target_link_libraries(ModelsTest PRIVATE Models gtest_main)

# ctest
add_test(NAME ModelsTest COMMAND ModelsTest)