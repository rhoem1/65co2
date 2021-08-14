
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, INS_ASL_IM)
{
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_ASL);
  executeCycles(2);
  if (A & 0x80)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(loByte(A << 1));
  expectRegisters(A << 1, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ASL_ZP)
{
  val = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_ASL_ZP, zeroPageAddress);
  executeCycles(5);
  expectAddressEquals(zeroPageAddress, val << 1);
  if (val & 0x80)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(loByte(val << 1));
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ASL_ZPX)
{
  val = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(loByte(zeroPageAddress + X), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_ASL_ZPX, zeroPageAddress);
  executeCycles(6);
  expectAddressEquals(loByte(zeroPageAddress + X), val << 1);
  if (val & 0x80)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(loByte(val << 1));
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ASL_ABS)
{
  val = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_ASL_ABS, addr);
  executeCycles(6);
  expectAddressEquals(addr, val << 1);
  if (val & 0x80)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(loByte(val << 1));
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ASL_ABSX)
{
  val = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr + X, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_ASL_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(7);
  else
    executeCycles(6);
  expectAddressEquals(addr + X, val << 1);
  if (val & 0x80)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(loByte(val << 1));
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}


