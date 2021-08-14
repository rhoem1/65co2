#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, LDYImmediate)
{
  Y = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  expectFlagsForValue(Y);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDYZeroPage)
{
  Y = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, Y);
  code(SixtyFiveCeeOhTwo::INS_LDY_ZP, zeroPageAddress);
  executeCycles(3);
  expectFlagsForValue(Y);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDYZeroPageX)
{
  X = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  cpu.write((zeroPageAddress + X) & 0xFF, Y);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDY_ZPX, zeroPageAddress);
  executeCycles(4);
  expectFlagsForValue(Y);
  expectRegisters(0, X, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDYAbsolute)
{
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, Y);
  code(SixtyFiveCeeOhTwo::INS_LDY_ABS, addr);
  executeCycles(4);
  expectFlagsForValue(Y);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDYAbsoluteX)
{
  X = randomByte();
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + X, Y);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDY_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectFlagsForValue(Y);
  expectRegisters(0, X, Y, 0xFD);
  expectPC();
}