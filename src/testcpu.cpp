
#include "gtest/gtest.h"
#include "Cpu/65co2.h"

#include "testcpu.h"


TEST_F(CpuTest, ResetCorrect)
{
	// flags
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_ZERO, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.SRgetByte(false), 0x24);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
	EXPECT_EQ(cpu.r.PC, 0x0200);

	// cycles
	EXPECT_EQ(cpu.r.cycles, 7);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
