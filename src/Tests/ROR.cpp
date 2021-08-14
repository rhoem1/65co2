
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, INS_ROR_IM)
{
  A = randomByte();
  cpu.r.SR_CARRY = (randomByte() & 0x01);
  val = (A >> 1) | (cpu.r.SR_CARRY ? 0x80 : 0x00);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_ROR);
  executeCycles(2);
  if (A & 0x01)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(val);
  expectRegisters(val, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ROR_ZP)
{
  val = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  cpu.r.SR_CARRY = (randomByte() & 0x01);
  uint8_t exval = (val >> 1) | (cpu.r.SR_CARRY ? 0x80 : 0x00);
  code(SixtyFiveCeeOhTwo::INS_ROR_ZP, zeroPageAddress);
  executeCycles(5);
  expectAddressEquals(zeroPageAddress, exval);
  if (val & 0x01)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(exval);
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ROR_ZPX)
{
  val = randomByte();
  X = randomByte();
  zeroPageAddress = randomByte();
  cpu.r.SR_CARRY = (randomByte() & 0x01);
  uint8_t exval = (val >> 1) | (cpu.r.SR_CARRY ? 0x80 : 0x00);
  cpu.write(loByte(zeroPageAddress + X), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_ROR_ZPX, zeroPageAddress);
  executeCycles(6);
  expectAddressEquals(loByte(zeroPageAddress + X), exval);
  if (val & 0x01)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(exval);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ROR_ABS)
{
  val = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.r.SR_CARRY = (randomByte() & 0x01);
  uint8_t exval = (val >> 1) | (cpu.r.SR_CARRY ? 0x80 : 0x00);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_ROR_ABS, addr);
  executeCycles(6);
  expectAddressEquals(addr, exval);
  if (val & 0x01)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(exval);
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_ROR_ABSX)
{
  val = randomByte();
  X = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.r.SR_CARRY = (randomByte() & 0x01);
  uint8_t exval = (val >> 1) | (cpu.r.SR_CARRY ? 0x80 : 0x00);
  cpu.write(addr + X, val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_ROR_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(7);
  else
    executeCycles(6);
  expectAddressEquals(addr + X, exval);
  if (val & 0x01)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  expectNoCarryFlagsForValue(exval);
  expectRegisters(0, X, 0, 0xFD);
  expectPC();
}