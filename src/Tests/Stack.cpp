#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, TransferSPToX)
{
  SP = randomByte();
  cpu.r.SP = SP;
  code(SixtyFiveCeeOhTwo::INS_TSX);
  executeCycles(2);
  expectFlagsForValue(SP);
  expectRegisters(0, SP, 0, SP);
  expectPC();
}

TEST_F(CpuTest, TransferXToSP)
{
  X = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  expectFlagsForValue(X);
  expectRegisters(0, X, 0, X);
  expectPC();
}

TEST_F(CpuTest, PushA)
{
  X = randomByte();
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  
  code(SixtyFiveCeeOhTwo::INS_PHA);
  executeCycles(3);
  
  expectAddressEquals(0x100 + X, A);

  expectFlagsForValue(A);
  expectRegisters(A, X, 0, X - 1);
  expectPC();
}

TEST_F(CpuTest, PullA)
{
  X = randomByte();
  val = randomByte();
  cpu.write(0x100 + ((X + 1) & 0xFF), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_PLA);
  executeCycles(4);
  expectFlagsForValue(val);
  expectRegisters(val, X, 0, X + 1);
  expectPC();
}

TEST_F(CpuTest, PushX)
{
  X = randomByte();
  val = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, val);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_PHX);
  executeCycles(3);
  expectAddressEquals(0x100 + X, val);
  expectFlagsForValue(val);
  expectRegisters(0, val, 0, X - 1);
  expectPC();
}

TEST_F(CpuTest, PullX)
{
  X = randomByte();
  val = randomByte();
  cpu.write(0x100 + ((X + 1) & 0xFF), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_PLX);
  executeCycles(4);
  expectFlagsForValue(val);
  expectRegisters(0, val, 0, X + 1);
  expectPC();
}

TEST_F(CpuTest, PushY)
{
  X = randomByte();
  val = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, val);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_PHY);
  executeCycles(3);
  expectAddressEquals(0x100 + X, val);
  expectFlagsForValue(val);
  expectRegisters(0, X, val, X - 1);
  expectPC();
}

TEST_F(CpuTest, PullY)
{
  X = randomByte();
  val = randomByte();
  cpu.write(0x100 + ((X + 1) & 0xFF), val);
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_PLY);
  executeCycles(4);
  expectFlagsForValue(val);
  expectRegisters(0, X, val, X + 1);
  expectPC();
}

TEST_F(CpuTest, PushStatus)
{
  X = randomByte();
  val = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);

  cpu.r.SRsetByte(val);
  code(SixtyFiveCeeOhTwo::INS_PHP);
  executeCycles(3);
  
  // check the flags written to the stack
  uint8_t r = cpu.readByte(0x100 + X);

	EXPECT_EQ(r & SR_NEGATIVE_B, val & SR_NEGATIVE_B);
	EXPECT_EQ(r & SR_ZERO_B, val & SR_ZERO_B);
	EXPECT_EQ(r & SR_OVERFLOW_B, val & SR_OVERFLOW_B);
	EXPECT_EQ(r & SR_DECIMAL_B, val & SR_DECIMAL_B);
	EXPECT_EQ(r & SR_CARRY_B, val & SR_CARRY_B);
	EXPECT_EQ(r & SR_INTERRUPT_B, val & SR_INTERRUPT_B);
  EXPECT_EQ(r & SR_BREAK_B, SR_BREAK_B);
  EXPECT_EQ(r & SR_UNUSED_B, SR_UNUSED_B);

	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, X, 0, X - 1);
  expectPC();
}

TEST_F(CpuTest, PullStatus)
{
  X = randomByte();
  val = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXS);
  executeCycles(2);
  
  // write the random stack value
  cpu.write(0x100 + ((X + 1) & 0xFF), val);

  code(SixtyFiveCeeOhTwo::INS_PLP);
  executeCycles(4);
  
  // check how the status register was set
	EXPECT_EQ(cpu.r.SR_NEGATIVE, (bool)(val & SR_NEGATIVE_B));
	EXPECT_EQ(cpu.r.SR_ZERO, (bool)(val & SR_ZERO_B));
	EXPECT_EQ(cpu.r.SR_OVERFLOW, (bool)(val & SR_OVERFLOW_B));
	EXPECT_EQ(cpu.r.SR_DECIMAL, (bool)(val & SR_DECIMAL_B));
	EXPECT_EQ(cpu.r.SR_CARRY, (bool)(val & SR_CARRY_B));
	EXPECT_EQ(cpu.r.SR_INTERRUPT, (bool)(val & SR_INTERRUPT_B));

	EXPECT_EQ(cpu.r.intb, false);

  expectRegisters(0, X, 0, X + 1);
  expectPC();
}
