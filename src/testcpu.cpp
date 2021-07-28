
#include "gtest/gtest.h"
#include "Cpu/65co2.h"

// The fixture for testing class Foo.
class CpuTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
  
  SixtyFiveCeeOhTwo cpu;

  CpuTest() {
     // You can do set-up work for each test here.
		 cpu.write(RESET_VECTOR, 0);
		 cpu.write(RESET_VECTOR + 1, 0x02);
     cpu.reset_cpu();
  }

  ~CpuTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

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
