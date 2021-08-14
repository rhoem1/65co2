#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"




TEST_F(CpuTest, STZZeroPage)
{
  while(val == 0)
    val = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_STZ_ZP, zeroPageAddress);
  executeCycles(3);
  expectAddressEquals(zeroPageAddress, 0);
  expectAddressNotEquals(zeroPageAddress, val);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STZZeroPageX)
{
  while(val == 0)
  val = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  addr = (zeroPageAddress + X) & 0xFF;
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STZ_ZPX, zeroPageAddress);
  executeCycles(4);
  expectAddressEquals(addr, 0);
  expectAddressNotEquals(addr, val);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STZAbsolute)
{
  while(val == 0)
  val = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_STZ_ABS, addr);
  executeCycles(4);
  expectAddressEquals(addr, 0);
  expectAddressNotEquals(addr, val);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, STZAbsoluteX)
{
  while(val == 0)
  val = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + X, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_STZ_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(6);
  else
    executeCycles(5);
  expectAddressEquals(addr + X, 0);
  expectAddressNotEquals(addr + X, val);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}
