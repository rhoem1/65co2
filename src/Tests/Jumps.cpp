
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

TEST_F(CpuTest, JumpAbsolute)
{
  addr = randomWordBetween(0x300, 0xC000);
  code(SixtyFiveCeeOhTwo::INS_JMP_ABS, addr);
  executeCycles(3);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFD);
  expectPC(addr);
}

TEST_F(CpuTest, JumpIndirect)
{
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(0x280, loByte(addr));
  cpu.write(0x281, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_JMP_IND, 0x280);
  executeCycles(5);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFD);
  expectPC(addr);
}

TEST_F(CpuTest, JumpIndirectX)
{
  addr = randomWordBetween(0x300, 0xC000);
  X = randomByte();
  cpu.write(0x280 + X, loByte(addr));
  cpu.write(0x281 + X, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_JMP_INDX, 0x280);
  executeCycles(6);
  expectFlagsForValue(X);
  expectPC(addr);
}

TEST_F(CpuTest, JumpSubroutine)
{
  addr = randomWordBetween(0x300, 0xC000);
  code(SixtyFiveCeeOhTwo::INS_JSR, addr);
  executeCycles(6);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFB);
  expectPC(addr);
  // look at what's put on the stack
  EXPECT_EQ(cpu.memory[0x1FD], loByte(expectedPC - 1));
  EXPECT_EQ(cpu.memory[0x1FC], hiByte(expectedPC - 1));
}

TEST_F(CpuTest, ReturnFromSubroutine)
{
  addr = randomWordBetween(0x300, 0xC000);
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_RTS);
  code(SixtyFiveCeeOhTwo::INS_JSR, addr);
  executeCycles(6);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFB);
  expectPC(addr);
  EXPECT_EQ(cpu.memory[0x1FD], loByte(expectedPC - 1));
  EXPECT_EQ(cpu.memory[0x1FC], hiByte(expectedPC - 1));
  // execute that RTS
  executeCycles(6);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}
