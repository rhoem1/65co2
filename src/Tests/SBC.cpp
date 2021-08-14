
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, INS_SBC_IM)
{
  val = randomByte();
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_IM, val);
  executeCycles(2);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_ZP)
{
  val = randomByte();
  A = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_ZP, zeroPageAddress);
  executeCycles(3);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_ZPI)
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
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_ZPI, zeroPageAddress);
  executeCycles(5);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_ZPX)
{
  val = randomByte();
  A = randomByte();
  X = randomByte();
  cpu.write(X, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_ZPX, 0x00);
  executeCycles(4);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_ABS)
{
  val = randomByte();
  A = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_ABS, addr);
  executeCycles(4);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_ABSX)
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
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_ABSY)
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
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_ABSY, addr);
  if(hiByte(addr + Y) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_INDX)
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
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_INDX, 0x00);
  executeCycles(6);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_SBC_INDY)
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
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_SBC_INDY, zeroPageAddress);
  if(hiByte(addr + Y - 1) != hiByte(addr))
    executeCycles(6);
  else
    executeCycles(5);
  expectSBCFlagsForValue(A - val);
  expectRegisters(loByte(A - val), 0, Y, 0xFD);
  expectPC();
}
