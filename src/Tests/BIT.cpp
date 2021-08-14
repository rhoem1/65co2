
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, INS_BIT_IM)
{
  val = randomByte();
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BIT_IM, val);
  executeCycles(2);
  expectBITFlagsForValueAndA(val, A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_BIT_ZP)
{
  val = randomByte();
  A = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BIT_ZP, zeroPageAddress);
  executeCycles(3);
  expectBITFlagsForValueAndA(val, A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_BIT_ZPX)
{
  val = randomByte();
  A = randomByte();
  X = randomByte();
  zeroPageAddress = 0x80;
  cpu.write(loByte(zeroPageAddress + X), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BIT_ZPX, zeroPageAddress);
  executeCycles(4);
  expectBITFlagsForValueAndA(val, A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_BIT_ABS)
{
  val = randomByte();
  A = randomByte();
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BIT_ABS, addr);
  executeCycles(4);
  expectBITFlagsForValueAndA(val, A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_BIT_ABSX)
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
  code(SixtyFiveCeeOhTwo::INS_BIT_ABSX, addr);
  if(hiByte(addr + X) != hiByte(addr))
    executeCycles(5);
  else
    executeCycles(4);
  expectBITFlagsForValueAndA(val, A);
  expectRegisters(A, X, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_TSB_ZP)
{
  val = randomByte();
  A = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TSB_ZP, zeroPageAddress);
  executeCycles(5);
  expectAddressEquals(zeroPageAddress, val | A);
  if(val & A)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);
  if(A & 0x80)
  EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
  EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_TSB_ABS)
{
  val = randomByte();
  A = randomByte();
  addr = randomByte();
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TSB_ABS, addr);
  executeCycles(6);
  expectAddressEquals(addr, val | A);
  if(val & A)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);
  if(A & 0x80)
  EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
  EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_TRB_ZP)
{
  val = randomByte();
  A = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TRB_ZP, zeroPageAddress);
  executeCycles(5);
  expectAddressEquals(zeroPageAddress, val & ~A);
  if(val & A)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);
  if(A & 0x80)
  EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
  EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_TRB_ABS)
{
  val = 0xFF; //randomByte();
  A = 0x01; //randomByte();
  addr = randomByte();
  cpu.write(addr, val);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TRB_ABS, addr);
  executeCycles(6);
  expectAddressEquals(addr, val & ~A);
  if(val & A)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);
  if(A & 0x80)
  EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
  EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
}