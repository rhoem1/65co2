#pragma once

#include <stdarg.h>

#include "gtest/gtest.h"
#include "Cpu/65co2.h"

// The fixture for testing class Foo.
class CpuTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  
  SixtyFiveCeeOhTwo cpu;
  
  uint8_t zeroPageAddress;
  uint8_t val;
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t SP;
  uint16_t addr;
  uint16_t expectedPC;
  
  const uint16_t STARTING_ADDRESS = 0x0200;

  CpuTest() {
     // You can do set-up work for each test here.
  }

  ~CpuTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override;

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
  
  uint8_t randomByte();
  
  uint16_t randomWordBetween(uint16_t start, uint16_t end);
  
  uint8_t loByte(uint16_t w) { return w & 0xFF; }
  uint8_t hiByte(uint16_t w) { return w >> 8; }

  void code(uint8_t op);
  void code(uint8_t op, uint16_t data);
  
  void executeCycles(uint64_t expectedCycles);

  void expectAddressEquals(uint16_t address, uint8_t testValue);

  void expectAddressNotEquals(uint16_t address, uint8_t testValue);
  
  void expectFalseFlags();
  
  void expectFlagsForValue(int16_t val);
  void expectADCFlagsForValue(int16_t val);
  void expectSBCFlagsForValue(int16_t val);
  void expectBITFlagsForValueAndA(int16_t val, uint8_t A);
  void expectNoCarryFlagsForValue(int16_t val);

  
  void expectRegisters(uint8_t A, uint8_t X, uint8_t Y, uint8_t SP);
  
  void expectPC();

  void expectPC(uint16_t PC);
  void executeAndExpectBranchPC(uint8_t branchAddress, bool branch);

};