#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, STYZeroPage)
{
  Y = randomByte();
  zeroPageAddress = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STY_ZP, zeroPageAddress);
  executeCycles(3);
  expectAddressEquals(zeroPageAddress, Y);
  expectFlagsForValue(Y);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STYZeroPageX)
{
  X = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  addr = (zeroPageAddress + X) & 0xFF;
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STY_ZPX, zeroPageAddress);
  executeCycles(4);
  expectAddressEquals(addr, Y);
  expectFlagsForValue(Y);
  expectRegisters(0, X, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STYAbsolute)
{
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STY_ABS, addr);
  executeCycles(4);
  expectAddressEquals(addr, Y);
  expectFlagsForValue(Y);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}
