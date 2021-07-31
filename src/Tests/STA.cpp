#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"




TEST_F(CpuTest, STAZeroPage)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();

  // lda #val
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  // sta $zp
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_ZP);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, STAZeroPageX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t x = randomByte();
  uint8_t zp = randomByte();
  
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  // sta $zpx
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_ZPX);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, STAAbsolute)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_ABS);
  addr++;
  cpu.write(addr, 0x00);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 4);

  uint8_t r = cpu.readByte(0x8000);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, STAAbsoluteX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t x = randomByte();
  
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_ABSX);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cpu.write(addr, 0x80);
  addr++;

  cycles = cpu.do_cycle();
  if(x > 0x7F)
  EXPECT_EQ(cycles, 6);
  else
  EXPECT_EQ(cycles, 5);

  uint8_t r = cpu.readByte(0x8080 + x);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, STAAbsoluteY)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t y = randomByte();

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, y);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);
  
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_ABSY);
  addr++;
  cpu.write(addr, 0x80);
  addr++;
  cpu.write(addr, 0x80);
  addr++;

  cycles = cpu.do_cycle();
  if(y > 0x7F)
  EXPECT_EQ(cycles, 6);
  else
  EXPECT_EQ(cycles, 5);

  uint8_t r = cpu.readByte(0x8080 + y);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, y);
  EXPECT_EQ(cpu.r.PC, addr);

}

TEST_F(CpuTest, STAIndirectX)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t x = randomByte();

  // on the zero page set a pointer to 0x8000
  cpu.write(x, 0x00);
  cpu.write((x + 1) & 0xFF, 0x80);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDX_IM);
  addr++;
  cpu.write(addr, x);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_INDX);
  addr++;
  cpu.write(addr, 0x00);
  addr++;

  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 6);

  uint8_t r = cpu.readByte(0x8000);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, x);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, STAIndirectY)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t y = randomByte();
  uint8_t zp = randomByte();

  cpu.write(zp, 0x80);
  cpu.write((zp + 1) & 0xFF, 0x80);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDY_IM);
  addr++;
  cpu.write(addr, y);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_INDY);
  addr++;
  cpu.write(addr, zp);
  addr++;
  cycles = cpu.do_cycle();
  if(y > 127)
  EXPECT_EQ(cycles, 7);
  else
  EXPECT_EQ(cycles, 6);

  uint8_t r = cpu.readByte(0x8080 + y);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, y);
  EXPECT_EQ(cpu.r.PC, addr);

}


TEST_F(CpuTest, STAZeroPageI)
{
  uint16_t addr = 0x200;
  uint64_t cycles = 0;
  uint8_t val = randomByte();
  uint8_t zp = randomByte();

  cpu.write(zp, 0x00);
  cpu.write((zp + 1) & 0xFF, 0x80);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  addr++;
  cpu.write(addr, val);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 2);

  cpu.write(addr, SixtyFiveCeeOhTwo::INS_STA_ZPI);
  addr++;
  cpu.write(addr, zp);
  addr++;
  cycles = cpu.do_cycle();
  EXPECT_EQ(cycles, 5);

  uint8_t r = cpu.readByte(0x8000);
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
	EXPECT_EQ(cpu.r.A, val);
	EXPECT_EQ(cpu.r.X, 0);
	EXPECT_EQ(cpu.r.Y, 0);
  EXPECT_EQ(cpu.r.PC, addr);

}