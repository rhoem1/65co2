#pragma once

#include "gtest/gtest.h"
#include "Cpu/65co2.h"

// The fixture for testing class Foo.
class CpuTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  
  SixtyFiveCeeOhTwo cpu;

  CpuTest() {
     // You can do set-up work for each test here.
     // set reset vector to 0x0200
     // any test code should execute from 0x0200 onward
     cpu.init_memory();
		 cpu.write(RESET_VECTOR, 0);
		 cpu.write(RESET_VECTOR + 1, 0x02);
     cpu.reset_cpu();
  }

  ~CpuTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
  
  uint8_t randomByte()
  {
    return rand() & 0xFF;
  }
};