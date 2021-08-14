#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, STXZeroPage)
{
  X = randomByte();
  zeroPageAddress = randomByte();

  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);

  code(SixtyFiveCeeOhTwo::INS_STX_ZP, zeroPageAddress);
  executeCycles(3);

  expectAddressEquals(zeroPageAddress, X);
  
  expectFlagsForValue(X);

  expectRegisters(0, X, 0, 0xFD);
  expectPC();
  
}

TEST_F(CpuTest, STXZeroPageY)
{
  X = randomByte();
  Y = randomByte();
  zeroPageAddress = randomByte();

  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);

  code(SixtyFiveCeeOhTwo::INS_STX_ZPY, zeroPageAddress);
  executeCycles(4);

  expectAddressEquals((zeroPageAddress + Y) & 0xFF, X);

  expectFlagsForValue(X);

  expectRegisters(0, X, Y, 0xFD);
  expectPC();
  
}

TEST_F(CpuTest, STXAbsolute)
{
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);

  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);

  code(SixtyFiveCeeOhTwo::INS_STX_ABS, addr);
  executeCycles(4);

  expectAddressEquals(addr, X);

  expectFlagsForValue(X);

  expectRegisters(0, X, 0, 0xFD);
  expectPC();

}
