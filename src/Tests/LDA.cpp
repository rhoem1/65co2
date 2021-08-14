#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, LDAImmediate)
{
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAZeroPage)
{
  A = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, A);
  code(SixtyFiveCeeOhTwo::INS_LDA_ZP, zeroPageAddress);
  executeCycles(3);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAZeroPageX)
{
  A = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  addr = (zeroPageAddress + X) & 0xFF;
  cpu.write(addr, A);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_ZPX, zeroPageAddress);
  executeCycles(4);
  expectFlagsForValue(A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAAbsolute)
{
  A = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, A);
  code(SixtyFiveCeeOhTwo::INS_LDA_ABS, addr);
  executeCycles(4);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAAbsoluteX)
{
  A = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + X, A);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectFlagsForValue(A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAAbsoluteY)
{
  A = randomByte();
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + Y, A);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_ABSY, addr);
  if(hiByte(addr + Y) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectFlagsForValue(A);
  expectRegisters(A, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAIndirectX)
{
  A = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, A);
  cpu.write(X, loByte(addr));
  cpu.write((X + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_INDX, 0x00);
  executeCycles(6);
  expectFlagsForValue(A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}


TEST_F(CpuTest, LDAIndirectY)
{
  A = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + Y, A);
  cpu.write(zeroPageAddress, loByte(addr));
  cpu.write((zeroPageAddress + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_INDY, zeroPageAddress);
  if(hiByte(addr + Y - 1) != hiByte(addr))
    executeCycles(6);
  else
    executeCycles(5);
  expectFlagsForValue(A);
  expectRegisters(A, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, LDAZeroPageI)
{
  A = randomByte();
  zeroPageAddress = 0xFF; //randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, A);
  cpu.write(zeroPageAddress, loByte(addr));
  cpu.write((zeroPageAddress + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDA_ZPI, zeroPageAddress);
  executeCycles(5);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
