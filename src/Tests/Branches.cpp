
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

// Zero flag
TEST_F(CpuTest, INS_BEQ)
{
  A = randomByte();
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BEQ, addr);
  executeAndExpectBranchPC(addr, cpu.r.SR_ZERO);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
}
TEST_F(CpuTest, INS_BNE)
{
  A = randomByte();
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BNE, addr);
  executeAndExpectBranchPC(addr, !cpu.r.SR_ZERO);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
}

// Carry
TEST_F(CpuTest, INS_BCS)
{
  addr = randomByte();
  if(addr & 1)
  code(SixtyFiveCeeOhTwo::INS_CLC);
  else
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BCS, addr);
  executeAndExpectBranchPC(addr, cpu.r.SR_CARRY);
  expectRegisters(0, 0, 0, 0xFD);
}
TEST_F(CpuTest, INS_BCC)
{
  addr = randomByte();
  if(addr & 1)
  code(SixtyFiveCeeOhTwo::INS_CLC);
  else
  code(SixtyFiveCeeOhTwo::INS_SEC);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BCC, addr);
  executeAndExpectBranchPC(addr, !cpu.r.SR_CARRY);
  expectRegisters(0, 0, 0, 0xFD);
}

// NEGATIVE flag
TEST_F(CpuTest, INS_BMI)
{
  A = randomByte();
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BMI, addr);
  executeAndExpectBranchPC(addr, cpu.r.SR_NEGATIVE);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
}
TEST_F(CpuTest, INS_BPL)
{
  A = randomByte();
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BPL, addr);
  executeAndExpectBranchPC(addr, !cpu.r.SR_NEGATIVE);
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
}

// overflow
TEST_F(CpuTest, INS_BVC)
{
  A = 0xFF;
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  if (addr & 1)
    code(SixtyFiveCeeOhTwo::INS_BIT_IM, 0x40);
  else
    code(SixtyFiveCeeOhTwo::INS_BIT_IM, 0x00);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BVC, addr);
  executeAndExpectBranchPC(addr, !cpu.r.SR_OVERFLOW);
  expectRegisters(A, 0, 0, 0xFD);
}
TEST_F(CpuTest, INS_BVS)
{
  A = 0xFF;
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  executeCycles(2);
  if (addr & 1)
    code(SixtyFiveCeeOhTwo::INS_BIT_IM, 0x40);
  else
    code(SixtyFiveCeeOhTwo::INS_BIT_IM, 0x00);
  executeCycles(2);
  code(SixtyFiveCeeOhTwo::INS_BVS, addr);
  executeAndExpectBranchPC(addr, cpu.r.SR_OVERFLOW);
  expectRegisters(A, 0, 0, 0xFD);
}

// always
TEST_F(CpuTest, INS_BRA)
{
  uint16_t dest;
  addr = randomByte();
  code(SixtyFiveCeeOhTwo::INS_BRA, addr);
  if(addr > 0x7F) 
    dest = expectedPC - (256 - addr);
  else
    dest = expectedPC + addr;
  if(hiByte(dest) != hiByte(expectedPC))
    executeCycles(5);
  else
    executeCycles(4);
  if(addr > 0x7F) 
    expectPC(expectedPC - (256 - addr));
  else
    expectPC(expectedPC + addr);
  expectRegisters(0, 0, 0, 0xFD);
}


TEST_F(CpuTest, INS_BBS0)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS0, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x01);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BSB1)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS1, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x02);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBS2)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS2, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x04);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBS3)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS3, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x08);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBS4)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS4, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x10);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBS5)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS5, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x20);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBS6)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS6, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x40);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBS7)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBS7, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, val & 0x80);
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR0)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR0, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x01));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR1)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR1, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x02));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR2)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR2, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x04));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR3)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR3, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x08));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR4)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR4, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x10));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR5)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR5, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x20));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR6)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR6, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x40));
  expectRegisters(0, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_BBR7)
{
  val = randomByte();
  addr = randomByte();
  zeroPageAddress = randomByte();
  cpu.write(zeroPageAddress, val);
  code(SixtyFiveCeeOhTwo::INS_BBR7, (addr << 8) + zeroPageAddress);
  executeAndExpectBranchPC(addr, !(val & 0x80));
  expectRegisters(0, 0, 0, 0xFD);
}