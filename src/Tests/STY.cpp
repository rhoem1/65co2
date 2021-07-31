#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"


TEST_F(CpuTest, STYZeroPage)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STY_ZP);
  addr++;
  cpu.write(addr, zp);
  addr++;
  
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 3);

  uint8_t r = cpu.readByte(zp);
  EXPECT_EQ(r, val);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, STYZeroPageX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();
  uint8_t x = randomByte();


  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;

  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STY_ZPX);
  addr++;
  cpu.write(addr, zp);
  addr++;
  
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 4);

  uint8_t r = cpu.readByte((zp + x) & 0xFF);
  EXPECT_EQ(r, val);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}

TEST_F(CpuTest, STYAbsolute)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();


  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STY_ABS);
  addr++;
  cpu.write(addr, zp);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 4);

  uint8_t r = cpu.readByte(0x8000 + zp);
  EXPECT_EQ(r, val);

  if(val > 127)
	EXPECT_EQ(cpu.r.SR_NEGATIVE, true);
  else
	EXPECT_EQ(cpu.r.SR_NEGATIVE, false);
  
  
  if(val != 0)
	EXPECT_EQ(cpu.r.SR_ZERO, false);
  else
	EXPECT_EQ(cpu.r.SR_ZERO, true);

	EXPECT_EQ(cpu.r.SR_OVERFLOW, false);
	EXPECT_EQ(cpu.r.SR_DECIMAL, false);
	EXPECT_EQ(cpu.r.SR_CARRY, false);
	EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
	EXPECT_EQ(cpu.r.intb, false);

	// registers
	EXPECT_EQ(cpu.r.SP, 0xFD);
	EXPECT_EQ(cpu.r.A, 0);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, val);
  EXPECT_EQ(cpu.r.PC, addr);
  
}
