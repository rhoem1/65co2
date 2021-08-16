
#include "gtest/gtest.h"
#include "Cpu/65co2.h"
#include "testcpu.h"

      //misc
TEST_F(CpuTest, INS_NOP) {
  code(SixtyFiveCeeOhTwo::INS_NOP);
  executeCycles(2);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFD);
  expectPC();
}

TEST_F(CpuTest, INS_BRK) {
  
  // breaks happen even when maskables are masked
  code(SixtyFiveCeeOhTwo::INS_SEI);
  executeCycles(2);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);

  // make 256 to 2048 nop ops, to move expectedPC to a random place
  addr = randomWordBetween(0x100, 0x800);
  do {
    code(SixtyFiveCeeOhTwo::INS_NOP);
    executeCycles(2);
    addr--;
  } while(addr > 0);
  addr = randomWordBetween(0xC000, 0xF000);
  cpu.write(IRQBRK_VECTOR, loByte(addr));
  cpu.write(IRQBRK_VECTOR + 1, hiByte(addr));
  code(SixtyFiveCeeOhTwo::INS_BRK);
  executeCycles(7);
  expectFalseFlags();
  expectRegisters(0, 0, 0, 0xFA);
  // stack should have next PC after break and specific flags
  expectAddressEquals(0x1FD, hiByte(expectedPC + 1));
  expectAddressEquals(0x1FC, loByte(expectedPC + 1));
  // breaks force the situation
  expectAddressEquals(0x1FB, SR_UNUSED_B | SR_INTERRUPT_B | SR_BREAK_B);
  expectPC(addr);
}

TEST_F(CpuTest, INS_RTI) {
  A = randomByte() & 0x7F;

  // make 256 to 2048 nop ops, to move expectedPC to a random place
  addr = randomWordBetween(0x100, 0x800);
  do {
    code(SixtyFiveCeeOhTwo::INS_NOP);
    executeCycles(2);
    addr--;
  } while(addr > 0);

  // this code should not execute
  uint16_t here = expectedPC;
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A | 0x80);
  code(SixtyFiveCeeOhTwo::INS_NOP);

  addr = randomWordBetween(0xC000, 0xF000);
  cpu.write(NMI_VECTOR, loByte(addr));
  cpu.write(NMI_VECTOR + 1, hiByte(addr));
  
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  cpu.write(addr + 1, A);
  cpu.write(addr + 2, SixtyFiveCeeOhTwo::INS_RTI);

  cpu.non_maskable_interrupt();

  // the irq happes at the end of do_op, so the lda executes, then the irq happens
  executeCycles(2 + 8); 
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFA);
  // stack should have PC when NMI happened
  expectAddressEquals(0x1FD, hiByte(here));
  expectAddressEquals(0x1FC, loByte(here));
  expectAddressEquals(0x1FB, SR_UNUSED_B | SR_INTERRUPT_B);
  
  // address of the RTI, not the NOP 
  expectPC(addr + 2);
  
  // should be the RTI
  executeCycles(6);
  // A should stet in the code at NMI_VECTOR
  expectRegisters(A, 0, 0, 0xFD);
  expectPC(here);
  
  // now give me the other LDA
  executeCycles(2); 
  expectFlagsForValue(A | 0x80);
  expectRegisters(A | 0x80, 0, 0, 0xFD);
  
}

TEST_F(CpuTest, IRQ) {
  A = randomByte() & 0x7F;

  addr = randomWordBetween(0xC000, 0xF000);
  cpu.write(IRQBRK_VECTOR, loByte(addr));
  cpu.write(IRQBRK_VECTOR + 1, hiByte(addr));
  
  // this code should execute
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  cpu.write(addr + 1, A);
  cpu.write(addr + 2, SixtyFiveCeeOhTwo::INS_CLI);
  cpu.write(addr + 3, SixtyFiveCeeOhTwo::INS_RTI);

  // clear interrupt masking
  code(SixtyFiveCeeOhTwo::INS_CLI);
  executeCycles(2);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, false);

  code(SixtyFiveCeeOhTwo::INS_NOP);
  uint16_t here = expectedPC;

  // this code should not execute
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A | 0x80);
  code(SixtyFiveCeeOhTwo::INS_NOP);

  cpu.maskable_interrupt(this);

  // still not masking
  EXPECT_EQ(cpu.r.SR_INTERRUPT, false);
  // intb is high
  EXPECT_EQ(cpu.r.intb, true);

  // cycles burned
  executeCycles(2 + 8); 
  // should not execute the LDA, just the NOP then IRQ
  
  // jump to the isr
  expectPC(addr);
  
  // now masking other interrupts
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, true);

  expectRegisters(0, 0, 0, 0xFA);
  // stack should have PC when NMI happened
  expectAddressEquals(0x1FD, hiByte(here));
  expectAddressEquals(0x1FC, loByte(here));
  // IRQs are not Breaks
  expectAddressEquals(0x1FB, SR_UNUSED_B);

  // execute the LDA
  executeCycles(2);
  expectRegisters(A, 0, 0, 0xFA);
  
  cpu.clear_maskable_interrupt(this);
  // intb is low
  EXPECT_EQ(cpu.r.intb, false);
  // still masking tho!
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);

  // CLI
  executeCycles(2);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, false);

  // RTI
  executeCycles(6);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC(here);

  // LDA with flag
  executeCycles(2);
  expectRegisters(A | 0x80, 0, 0, 0xFD);

}


TEST_F(CpuTest, MaskedIRQ) {
  A = randomByte() & 0x7F;

  addr = randomWordBetween(0xC000, 0xF000);
  cpu.write(IRQBRK_VECTOR, loByte(addr));
  cpu.write(IRQBRK_VECTOR + 1, hiByte(addr));
  
  // this code should not be called
  cpu.write(addr, SixtyFiveCeeOhTwo::INS_LDA_IM);
  cpu.write(addr + 1, A | 0x80);
  cpu.write(addr + 2, SixtyFiveCeeOhTwo::INS_RTI);

  // mask irqs
  code(SixtyFiveCeeOhTwo::INS_SEI);
  executeCycles(2);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);

  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);

  EXPECT_EQ(cpu.r.intb, false);
  // the interrupt should be ignored
  cpu.maskable_interrupt(this);
  EXPECT_EQ(cpu.r.SR_INTERRUPT, true);
  EXPECT_EQ(cpu.r.intb, true);
  
  // even tho intb is high, do not jump to IRQ routine
  executeCycles(2);
  
  expectFlagsForValue(A);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();

  cpu.clear_maskable_interrupt(this);
  EXPECT_EQ(cpu.r.intb, false);

}

TEST_F(CpuTest, INS_STP)
{
  A = randomByte() & 0x7F;
  EXPECT_EQ(cpu.r.stopped, false);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  code(SixtyFiveCeeOhTwo::INS_STP);
  // LDA
  executeCycles(2);
  expectRegisters(A, 0, 0, 0xFD);
  // STP
  executeCycles(3);
  expectPC();
  // is stopped?
  EXPECT_EQ(cpu.r.stopped, true);
  executeCycles(0);
  expectPC();
  // reset
  cpu.reset_cpu();
  expectRegisters(0, 0, 0, 0xFD);
  // reset + LDA
  executeCycles(9);
  expectRegisters(A, 0, 0, 0xFD);
}

TEST_F(CpuTest, INS_WAI)
{
  A = randomByte() & 0x7F;
  EXPECT_EQ(cpu.r.waiting, false);
  code(SixtyFiveCeeOhTwo::INS_SEI);
  code(SixtyFiveCeeOhTwo::INS_WAI);
  code(SixtyFiveCeeOhTwo::INS_LDA_IM, A);
  
  // if interrupts are masked, and WAI is executed
  // the cpu waits.  when an IRQ fires, then wait ends
  // and the PC should move to the next op after the WAI
  // and not the IRQ SVR
  
  // SEI
  executeCycles(2);
  // WAI
  executeCycles(3);
  
  // no cycles execute
  EXPECT_EQ(cpu.r.waiting, true);
  executeCycles(0);
  
  // IRQ!
  cpu.maskable_interrupt(this);
  EXPECT_EQ(cpu.r.waiting, false);

  // LDA
  executeCycles(2);
  expectRegisters(A, 0, 0, 0xFD);
  expectPC();
  
}