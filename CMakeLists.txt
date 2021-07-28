cmake_minimum_required(VERSION 3.10)


project(SixtyFiveCeeOhTwo VERSION 1.0)


# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
#FetchContent_Populate(googletest)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



include_directories(${SixtyFiveCeeOhTwo_SOURCE_DIR}/src)
link_directories(${SixtyFiveCeeOhTwo_BINARY_DIR}/build)
add_executable(apple1 src/main.cpp src/apple1stdio.cpp src/apple1.cpp src/Cpu/65co2.cpp)

add_executable(testcpu src/testcpu.cpp src/Cpu/65co2.cpp)
target_link_libraries(testcpu gtest_main)
#include(googletest)
#gtest_discover_tests(testcpu)

