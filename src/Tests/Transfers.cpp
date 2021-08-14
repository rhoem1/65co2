
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

      //Transfer Registers
TEST_F(CpuTest, INS_TAX)
{
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TAX);
  executeCycles(2);
  expectFlagsForValue(A);
  expectRegisters(A, A, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_TAY)
{
  A = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TAY);
  executeCycles(2);
  expectFlagsForValue(A);
  expectRegisters(A, 0, A, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_TXA)
{
  X = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDX_IM, X);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TXA);
  executeCycles(2);
  expectFlagsForValue(X);
  expectRegisters(X, X, 0, 0xFD);
  expectPC();
}
TEST_F(CpuTest, INS_TYA)
{
  Y = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDY_IM, Y);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_TYA);
  executeCycles(2);
  expectFlagsForValue(Y);
  expectRegisters(Y, 0, Y, 0xFD);
  expectPC();
}