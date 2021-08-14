#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"




TEST_F(CpuTest, STAZeroPage)
{
  A = randomByte();
  zeroPageAddress = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_ZP, zeroPageAddress);
  executeCycles(3);
  expectAddressEquals(zeroPageAddress, A);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STAZeroPageX)
{
  A = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_ZPX, zeroPageAddress);
  executeCycles(4);
  expectAddressEquals((zeroPageAddress + X) & 0xFF, A);
  expectFlagsForValue(A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}


TEST_F(CpuTest, STAAbsolute)
{
  A = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_ABS, addr);
  executeCycles(4);
  expectAddressEquals(addr, A);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STAAbsoluteX)
{
  A = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(6);
  else
    executeCycles(5);
  expectAddressEquals(addr + X, A);
  expectFlagsForValue(A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STAAbsoluteY)
{
  A = randomByte();
  Y = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_ABSY, addr);
  if(hiByte(addr + Y) != hiByte(addr))
    executeCycles(6);
  else
    executeCycles(5);
  expectAddressEquals(addr + Y, A);
  expectFlagsForValue(A);
  expectRegisters(A, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STAIndirectX)
{
  A = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(X, loByte(addr));
  cpu.write((X + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_INDX, 0x00);
  executeCycles(6);
  expectAddressEquals(addr, A);
  expectFlagsForValue(A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STAIndirectY)
{
  A = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(zeroPageAddress, loByte(addr));
  cpu.write((zeroPageAddress + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_INDY, zeroPageAddress);
  if(hiByte(addr + Y - 1) != hiByte(addr))
    executeCycles(7);
  else
    executeCycles(6);
  expectAddressEquals(addr + Y, A);
  expectFlagsForValue(A);
  expectRegisters(A, 0, Y, 0xFD);
  expectPC();
}


TEST_F(CpuTest, STAZeroPageI)
{
  A = randomByte();
  zeroPageAddress = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(zeroPageAddress, loByte(addr));
  cpu.write((zeroPageAddress + 1) & 0xFF, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STA_ZPI, zeroPageAddress);
  executeCycles(5);
  expectAddressEquals(addr, A);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
