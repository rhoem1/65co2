
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, ClearCarry)
{
  cpu.r.SR_CARRY = true;
  code(SixtyFiveCeeOhTwo::INS_CLC);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, SetCarry)
{
  cpu.r.SR_CARRY = false;
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, true);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, ClearDecimal)
{
  cpu.r.SR_DECIMAL = true;
  code(SixtyFiveCeeOhTwo::INS_CLD);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, SetDecimal)
{
  cpu.r.SR_DECIMAL = false;
  code(SixtyFiveCeeOhTwo::INS_SED);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, true);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, ClearInterrupt)
{
  cpu.r.SR_INTERRUPT = true; // likely still true from reset
  code(SixtyFiveCeeOhTwo::INS_CLI);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, false);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, SetInterrupt)
{
  cpu.r.SR_INTERRUPT = false; 
  code(SixtyFiveCeeOhTwo::INS_SEI);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, ClearOverflow)
{
  cpu.r.SR_OVERFLOW = true;
  code(SixtyFiveCeeOhTwo::INS_CLV);
  executeCycles(2);

	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}
