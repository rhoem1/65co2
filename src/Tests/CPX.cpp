
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, INS_CPX_IM)
{
  val = randomByte();
  X = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_CPX_IM, val);
  executeCycles(2);
  expectSBCFlagsForValue(X - val);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_CPX_ZP)
{
  val = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_CPX_ZP, zeroPageAddress);
  executeCycles(3);
  expectSBCFlagsForValue(X - val);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_CPX_ABS)
{
  val = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_CPX_ABS, addr);
  executeCycles(4);
  expectSBCFlagsForValue(X - val);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}
