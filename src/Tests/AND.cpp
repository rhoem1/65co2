
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, INS_AND_IM)
{
  val = randomByte();
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_IM, val);
  executeCycles(2);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_ZP)
{
  val = randomByte();
  A = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_ZP, zeroPageAddress);
  executeCycles(3);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_ZPI)
{
  val = randomByte();
  A = randomByte();
  zeroPageAddress = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  cpu.write(zeroPageAddress, loByte(addr));
  cpu.write((zeroPageAddress + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_ZPI, zeroPageAddress);
  executeCycles(5);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_ZPX)
{
  val = randomByte();
  A = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(loByte(zeroPageAddress + X), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_ZPX, zeroPageAddress);
  executeCycles(4);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_ABS)
{
  val = randomByte();
  A = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_ABS, addr);
  executeCycles(4);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_ABSX)
{
  val = randomByte();
  A = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + X, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_ABSY)
{
  val = randomByte();
  A = randomByte();
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + Y, val);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_ABSY, addr);
  if(hiByte(addr + Y) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_INDX)
{
  val = randomByte();
  A = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  cpu.write(X, loByte(addr));
  cpu.write((X + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_INDX, 0x00);
  executeCycles(6);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_AND_INDY)
{
  val = randomByte();
  A = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + Y, val);
  cpu.write(zeroPageAddress, loByte(addr));
  cpu.write((zeroPageAddress + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_AND_INDY, zeroPageAddress);
  if(hiByte(addr + Y - 1) != hiByte(addr))
    executeCycles(6);
  else
    executeCycles(5);
  expectFlagsForValue(A & val);
  expectRegisters(A & val, 0, Y, 0xFD);
  expectPC();
}
