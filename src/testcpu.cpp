
#include "gtest/gtest.h"
#include "Cpu/65co2.h"

#include "testcpu.h"

void CpuTest::SetUp()
{
  // Code here will be called immediately after the constructor (right
  // before each test).

  cpu.init_memory();
  cpu.write(RESET_VECTOR, STARTING_ADDRESS & 0xFF);
  cpu.write(RESET_VECTOR + 1, STARTING_ADDRESS >> 8);
  cpu.reset_cpu();

  zeroPageAddress = 0;
  val = 0;
  A = 0;
  X = 0;
  Y = 0;
  SP = 0xFD;
  addr = 0;
  expectedPC = STARTING_ADDRESS;
}
uint8_t CpuTest::randomByte()
{
  return rand() & 0xFF;
}

uint16_t CpuTest::randomWordBetween(uint16_t start, uint16_t end)
{
  uint16_t range = end - start;
  return (rand() % range) + start;
}

void CpuTest::code(uint8_t op)
{
  cpu.write(expectedPC, op);
  expectedPC++;
}
/**
   * Push some code and data at expectedPC
   * @param uint8_t op Operation
   * @param uint16_t data
   */
void CpuTest::code(uint8_t op, uint16_t data)
{

  code(op);

  switch (cpu.OPCODES[op].step)
  {
  case 1:
    cpu.write(expectedPC, data);
    expectedPC++;
    break;
  case 2:
    cpu.write(expectedPC, loByte(data));
    expectedPC++;
    cpu.write(expectedPC, hiByte(data));
    expectedPC++;
    break;
  }
}

void CpuTest::executeCycles(uint64_t expectedCycles)
{
  uint64_t actualCycles = cpu.do_cycle();
  EXPECT_EQ(actualCycles, expectedCycles);
}

void CpuTest::expectAddressEquals(uint16_t address, uint8_t testValue)
{
  uint8_t valueAtAddress = cpu.readByte(address);
  EXPECT_EQ(valueAtAddress, testValue);
}

void CpuTest::expectAddressNotEquals(uint16_t address, uint8_t testValue)
{
  uint8_t valueAtAddress = cpu.readByte(address);
  EXPECT_NE(valueAtAddress, testValue);
}

void CpuTest::expectFalseFlags()
{
  EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  EXPECT_EQ(cpu.r.SR_ZERO, false);
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
}

void CpuTest::expectFlagsForValue(int16_t testValue)
{
  if (testValue & 0x80)
    EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
    EXPECT_EQ(cpu.r.SR_NEGATIVE, false);

  if (testValue != 0)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);

  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
}

void CpuTest::expectBITFlagsForValueAndA(int16_t testValue, uint8_t A)
{
  if (testValue & 0x80)
    EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
    EXPECT_EQ(cpu.r.SR_NEGATIVE, false);

  if (testValue & 0x40)
    EXPECT_EQ(cpu.r.SR_OVERFLOW, true);
  else
    EXPECT_EQ(cpu.r.SR_OVERFLOW, false);

  if(testValue & A)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);

  EXPECT_EQ(cpu.r.SR_CARRY, false);
  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
}

void CpuTest::expectNoCarryFlagsForValue(int16_t testValue)
{
  if (testValue & 0x80)
    EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
    EXPECT_EQ(cpu.r.SR_NEGATIVE, false);

  if (testValue == 0)
    EXPECT_EQ(cpu.r.SR_ZERO, true);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);

  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
}

void CpuTest::expectADCFlagsForValue(int16_t testValue)
{
  if (testValue & 0x80)
    EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
    EXPECT_EQ(cpu.r.SR_NEGATIVE, false);

  if (testValue == 0 || testValue == 256)
    EXPECT_EQ(cpu.r.SR_ZERO, true);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, false);

  if (testValue > 255 || testValue < 0)
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  
  if (testValue > 255)
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);

  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
}

void CpuTest::expectSBCFlagsForValue(int16_t testValue)
{
  if (testValue & 0x80)
    EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
    EXPECT_EQ(cpu.r.SR_NEGATIVE, false);

  if (testValue != 0)
    EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
    EXPECT_EQ(cpu.r.SR_ZERO, true);

  if (testValue > 255 || testValue < 0)
    EXPECT_EQ(cpu.r.SR_CARRY, false);
  else
    EXPECT_EQ(cpu.r.SR_CARRY, true);
  
  if (testValue > 255)
  EXPECT_EQ(cpu.r.SR_OVERFLOW, false);

  EXPECT_EQ(cpu.r.SR_DECIMAL, false);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, false);
}

void CpuTest::expectRegisters(uint8_t A, uint8_t X, uint8_t Y, uint8_t SP)
{
  // registers
  EXPECT_EQ(cpu.r.SP, SP);
  EXPECT_EQ(cpu.r.A, A);
  EXPECT_EQ(cpu.r.X, X);
  EXPECT_EQ(cpu.r.Y, Y);
}

void CpuTest::expectPC()
{
  EXPECT_EQ(cpu.r.PC, expectedPC);
}

void CpuTest::expectPC(uint16_t PC)
{
  EXPECT_EQ(cpu.r.PC, PC);
}

void CpuTest::executeAndExpectBranchPC(uint8_t branchAddress, bool branch)
{
  if(branch) {
    uint16_t dest;
    if(branchAddress > 0x7F) 
      dest = expectedPC - (256 - branchAddress);
    else
      dest = expectedPC + branchAddress;
    if(hiByte(dest) != hiByte(expectedPC))
      executeCycles(4);
    else
      executeCycles(3);
    if(branchAddress > 0x7F) 
      expectPC(expectedPC - (256 - branchAddress));
    else
      expectPC(expectedPC + branchAddress);
  }
  else
  {
    executeCycles(2);
    expectPC();
  }
}

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
  srand(time(NULL));
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
