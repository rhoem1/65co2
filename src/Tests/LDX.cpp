#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, LDXImmediate)
{
  X = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDXZeroPage)
{
  X = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, X);
  code(SixtyFiveCeeOhTwo::INS_LDX_ZP, zeroPageAddress);
  executeCycles(3);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDXZeroPageY)
{
  X = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  cpu.write((zeroPageAddress + Y) & 0xFF, X);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDX_ZPY, zeroPageAddress);
  executeCycles(4);
  expectFlagsForValue(X);
  expectRegisters(0, X, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDXAbsolute)
{
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, X);
  code(SixtyFiveCeeOhTwo::INS_LDX_ABS, addr);
  executeCycles(4);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDXAbsoluteY)
{
  X = randomByte();
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + Y, X);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDX_ABSY, addr);
  if(hiByte(addr + Y) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectFlagsForValue(X);
  expectRegisters(0, X, Y, 0xFD);
  expectPC();
}
