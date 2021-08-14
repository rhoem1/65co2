
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, INS_CPY_IM)
{
  val = randomByte();
  Y = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_CPY_IM, val);
  executeCycles(2);
  expectSBCFlagsForValue(Y - val);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_CPY_ZP)
{
  val = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_CPY_ZP, zeroPageAddress);
  executeCycles(3);
  expectSBCFlagsForValue(Y - val);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_CPY_ABS)
{
  val = randomByte();
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_CPY_ABS, addr);
  executeCycles(4);
  expectSBCFlagsForValue(Y - val);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}