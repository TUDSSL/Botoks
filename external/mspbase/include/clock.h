#ifndef INCLUDE_MSPBASE_CLOCK_H
#define INCLUDE_MSPBASE_CLOCK_H

#define CLK_8_MHZ DCOFSEL_6

#define SMCLK_0_25_MHZ DIVS__32
#define SMCLK_0_5_MHZ DIVS__16
#define SMCLK_4_MHZ DIVS__2
#define SMCLK_8_MHZ DIVS__1

#define msp_clock_set_mclk(freq, div) \
  CSCTL0_H = CSKEY_H;                 \
  CSCTL1 = freq;                      \
  CSCTL3 &= ~(DIVS | DIVM);           \
  CSCTL3 |= div;                      \
  CSCTL0_H = 0

#endif /* INCLUDE_MSPBASE_CLOCK_H */
