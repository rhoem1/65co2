
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, INS_INX)
{
  X = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_INX);
  executeCycles(2);
  X = loByte(X + 1);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_INY)
{
  Y = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_INY);
  executeCycles(2);
  Y = loByte(Y + 1);
  expectFlagsForValue(Y);
  expectRegisters(0, 0, Y, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_INC_A)
{
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_INC_A);
  executeCycles(2);
  A = loByte(A + 1);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_INC_ZP)
{
  val = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_INC_ZP, zeroPageAddress);
  executeCycles(5);
  val = loByte(val + 1);
  expectAddressEquals(zeroPageAddress, val);
  expectFlagsForValue(val);
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_INC_ZPX)
{
  val = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(loByte(zeroPageAddress + X), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_INC_ZPX, zeroPageAddress);
  executeCycles(6);
  val = loByte(val + 1);
  expectAddressEquals(loByte(zeroPageAddress + X), val);
  expectFlagsForValue(val);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_INC_ABS)
{
  val = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_INC_ABS, addr);
  executeCycles(6);
  val = loByte(val + 1);
  expectAddressEquals(addr, val);
  expectFlagsForValue(val);
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_INC_ABSX)
{
  val = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + X, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_INC_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(8);
  else
    executeCycles(7);
  val = loByte(val + 1);
  expectAddressEquals(addr + X, val);
  expectFlagsForValue(val);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}