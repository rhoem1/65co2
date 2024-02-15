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

add_custom_target(MSBASIC
  COMMAND
    cd msbasic &&
    /opt/cc65/bin/ca65 -D applesoft msbasic.s -o tmp/applesoft.o &&
    /opt/cc65/bin/ld65 -C applesoft.cfg tmp/applesoft.o -o tmp/applesoft.bin -Ln tmp/applesoft.lbl
  DEPENDS 
     msbasic/apple_extra.s
     msbasic/apple_iscntc.s
     msbasic/apple_loadsave.s
     msbasic/applesoft.cfg
     msbasic/array.s
     msbasic/cbm1_patches.s
     msbasic/cbm_iscntc.s
     msbasic/cbmbasic1.cfg
     msbasic/cbmbasic2.cfg
     msbasic/chrget.s
     msbasic/defines.s
     msbasic/defines_apple.s
     msbasic/defines_cbm1.s
     msbasic/defines_cbm2.s
     msbasic/defines_kbd.s
     msbasic/defines_kim.s
     msbasic/defines_microtan.s
     msbasic/defines_osi.s
     msbasic/error.s
     msbasic/eval.s
     msbasic/extra.s
     msbasic/float.s
     msbasic/flow1.s
     msbasic/flow2.s
     msbasic/header.s
     msbasic/init.s
     msbasic/inline.s
     msbasic/input.s
     msbasic/iscntc.s
     msbasic/kb9.cfg
     msbasic/kbd_extra.s
     msbasic/kbd_iscntc.s
     msbasic/kbd_loadsave.s
     msbasic/kbdbasic.cfg
     msbasic/kim_extra.s
     msbasic/kim_iscntc.s
     msbasic/kim_loadsave.s
     msbasic/loadsave.s
     msbasic/macros.s
     msbasic/memory.s
     msbasic/message.s
     msbasic/microtan.cfg
     msbasic/microtan_extra.s
     msbasic/microtan_iscntc.s
     msbasic/microtan_loadsave.s
     msbasic/misc1.s
     msbasic/misc2.s
     msbasic/misc3.s
     msbasic/msbasic.s
     msbasic/osi.cfg
     msbasic/osi_iscntc.s
     msbasic/poke.s
     msbasic/print.s
     msbasic/program.s
     msbasic/rnd.s
     msbasic/string.s
     msbasic/token.s
     msbasic/trig.s
     msbasic/var.s
     msbasic/zeropage.s
)

add_custom_target(MSBASIC_CODE
  COMMAND php import_bin msbasic/tmp/applesoft.bin MSBASIC tmp_applesoft_bin > src/Roms/msbasic_code.h
  DEPENDS MSBASIC
)

include_directories(${SixtyFiveCeeOhTwo_SOURCE_DIR}/src)
link_directories(${SixtyFiveCeeOhTwo_BINARY_DIR}/build)
add_executable(
  apple1 
    src/main.cpp 
    src/apple1stdio.cpp 
    src/apple1.cpp 
    src/Cpu/65co2.cpp
)
add_dependencies(
  apple1
    MSBASIC_CODE
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


