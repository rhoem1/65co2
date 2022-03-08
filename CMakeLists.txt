cmake_minimum_required(VERSION 3.10)


project(SixtyFiveCeeOhTwo VERSION 1.0)

set(CMAKE_BUILD_TYPE Debug)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

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
add_executable(
  apple1 
    src/main.cpp 
    src/apple1stdio.cpp 
    src/apple1.cpp 
    src/Cpu/65co2.cpp
)

enable_testing()
add_executable(
  testcpu 
  src/testcpu.cpp 
  src/Cpu/65co2.cpp 
  src/Tests/LDX.cpp
  src/Tests/LDY.cpp
  src/Tests/LDA.cpp
  src/Tests/STX.cpp
  src/Tests/STY.cpp
  src/Tests/STA.cpp
  src/Tests/STZ.cpp

  src/Tests/DEC.cpp
  src/Tests/INC.cpp

  src/Tests/ADC.cpp
  src/Tests/SBC.cpp

  src/Tests/AND.cpp
  src/Tests/EOR.cpp
  src/Tests/ORA.cpp

  src/Tests/ASL.cpp
  src/Tests/LSR.cpp
  src/Tests/ROL.cpp
  src/Tests/ROR.cpp

  src/Tests/BIT.cpp
  src/Tests/CMP.cpp
  src/Tests/CPX.cpp
  src/Tests/CPY.cpp

  src/Tests/Stack.cpp
  src/Tests/StatusFlags.cpp
  src/Tests/Branches.cpp
  src/Tests/Jumps.cpp
  src/Tests/Transfers.cpp
  src/Tests/Misc.cpp
)
target_link_libraries(testcpu gtest_main)
include(GoogleTest)
gtest_discover_tests(testcpu)


