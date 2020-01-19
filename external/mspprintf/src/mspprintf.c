#include <msp430.h>
#include <mspprintf.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PUTC(c) io_putchar(c)

static uint8_t* tx_data;
static unsigned tx_len;
static volatile bool tx_finished;

void msp_uart_open() {
  P2SEL1 |= BIT0 | BIT1;  // USCI_A0 UART operation
  P2SEL0 &= ~(BIT0 | BIT1);

  UCA0CTLW0 |= UCSWRST;
  UCA0CTLW0 |=
      UCSSEL__SMCLK;  // this clock must be the same as the unified clock
  UCA0CTL1 &= ~UCSWRST;

  // Enable general interrupts.
  __enable_interrupt();
}

void msp_uart_close() { UCA0CTLW0 |= UCSWRST; }

void msp_uart_send_sync(uint8_t* payload, unsigned len) {
  if (len == 0) return;

  // Setup pointers for the ISR
  tx_data = payload;
  tx_len = len;
  tx_finished = false;
  UCA0IE |= UCTXIE;

// On the CC430 family, the ISR does not fire as a result of enabling IE
// (despite IFG being on). We need to write a byte to clear the IFG, and await
// the interrupt. Also, the TXIFG behavior is different, see ISR.
#if defined(__CC430__) || defined(__MSP430F__)
  --tx_len;
  UCA0TXBUF = *tx_data++;  // first byte, clears IFG
#endif

  // Wait while ISR TXes the remaining bytes
  //
#ifdef LIBMSP_SLEEP
  // We have to disable TX int from ISR, otherwise, will enter infinite ISR
  // loop.
  __disable_interrupt();  // classic lock-check-(sleep+unlock)-lock pattern
  while (!tx_finished) {
    __bis_SR_register(LPM0_bits + GIE);  // will wakeup after ISR TXes last byte
    __disable_interrupt();
  }
  __enable_interrupt();
#else   // ! LIBMSP_SLEEP
  while (!tx_finished)
    ;
#endif  // ! LIBMSP_SLEEP

  // TXCPTIFG (and TXIFG) both happen before the byte is
  // transfered... so have to busywait
  while (UCA0STATW & UCBUSY)
    ;
}

__attribute__((interrupt(EUSCI_A0_VECTOR))) void EUSCI_A0_ISR(void) {
  switch (__even_in_range(UCA0IV, 0x08)) {
    case UCIV__UCTXIFG:

// On CC430, the TXIFG fires when tx has finished, whereas
// on MSP430FR, TXIFG fires when is ready to accept the next byte, which
// happens before the last has finished transmitting byte. Hence, the
// logic in the ISR is different.
#if defined(__CC430__) || defined(__MSP430F__)
      if (tx_len--) {
        UCA0TXBUF = *tx_data++;
      } else {  // last byte got done
        tx_finished = true;
#ifdef LIBMSP_SLEEP
        __bic_SR_register_on_exit(LPM4_bits);  // wakeup
#endif                                         // LIBMSP_SLEEP
      }
#else   // !(__CC430__ || __MSP430F__)
      UCA0TXBUF = *tx_data++;
      if (--tx_len == 0) {
        UCA0IE &= ~UCTXIE;
        UCA0IFG &= ~UCTXCPTIFG;
        UCA0IE |= UCTXCPTIE;
      }
#endif  // !(__CC430__ || __MSP430F__)
      break;
    case UCIV__UCRXIFG:
      break;
    case UCIV__UCTXCPTIFG:
      tx_finished = true;
#ifdef LIBMSP_SLEEP
      __bic_SR_register_on_exit(LPM4_bits);  // wakeup
#endif                                       // LIBMSP_SLEEP
      break;
    default:
      break;
  }
}

int io_putchar(int c) {
  uint8_t ch = c;
  msp_uart_send_sync(&ch, 1);
  return c;
}

int io_puts_no_newline(const char* ptr) {
  unsigned len = 0;
  const char* p = ptr;

  while (*p++ != '\0') len++;

  msp_uart_send_sync((uint8_t*)ptr, len);
  return len;
}

int io_puts(const char* ptr) {
  unsigned len;

  len = io_puts_no_newline(ptr);

  // Semantics of puts are annoying...
  io_putchar('\n');

  return len;
}

/* Not used
static int msp_io_puts(const char *ptr)
{
    unsigned len;

    len = msp_io_puts_no_newline(ptr);

    msp_io_putchar('\n');

    return len;
}
*/

// Tiny printf implementation by oPossum from here:
// http://forum.43oh.com/topic/1289-tiny-printf-c-version/#entry10652
//
// NOTE: We are not using the libc printf because it's huge:
// https://e2e.ti.com/support/development_tools/compiler/f/343/t/442632

static const unsigned long dv[] = {
    //  4294967296      // 32 bit unsigned max
    1000000000,  // +0
    100000000,   // +1
    10000000,    // +2
    1000000,     // +3
    100000,      // +4
                 //       65535      // 16 bit unsigned max
    10000,       // +5
    1000,        // +6
    100,         // +7
    10,          // +8
    1,           // +9
};

static void xtoa(unsigned long x, const unsigned long* dp) {
  char c;
  unsigned long d;
  if (x) {
    while (x < *dp) ++dp;
    do {
      d = *dp++;
      c = '0';
      while (x >= d) ++c, x -= d;
      PUTC(c);
    } while (!(d & 1));
  } else
    PUTC('0');
}

static void puth(unsigned n) {
  static const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  PUTC(hex[n & 15]);
}

int msp_printf(const char* format, ...) {
  uint16_t smclkDivs = CSCTL3 & DIVS;
  switch (smclkDivs) {
    case DIVS__1:
      // 19200
      UCA0CTLW0 |= UCSWRST;
      UCA0BR0 = 26;
      UCA0MCTLW = UCOS16 | 0xB600 | 0x0000;
      UCA0BR1 = 0;
      UCA0CTL1 &= ~UCSWRST;
      break;
    case DIVS__16:
      // 19200
      UCA0CTLW0 |= UCSWRST;
      UCA0BR0 = 1;
      UCA0MCTLW = UCOS16 | 0x0100 | 0x00A0;
      UCA0BR1 = 0;
      UCA0CTL1 &= ~UCSWRST;
      break;
    case DIVS__32:
      // 19200
      UCA0CTLW0 |= UCSWRST;
      UCA0BR0 = 13;
      UCA0MCTLW = 0x0000 | 0x0000;
      UCA0BR1 = 0;
      UCA0CTL1 &= ~UCSWRST;
      break;
  }

  char c;
  int i;
  long n;
  int fill_zeros;
  unsigned d;

  va_list a;
  va_start(a, format);
  while ((c = *format++)) {
    if (c == '%') {
      fill_zeros = 0;
    parse_fmt_char:
      switch (c = *format++) {
        case 's':  // String
          io_puts_no_newline(va_arg(a, char*));
          break;
        case 'c':                // Char
          PUTC(va_arg(a, int));  // TODO: 'char' generated a warning
          break;
        case 'i':  // 16 bit Integer
        case 'u':  // 16 bit Unsigned
          i = va_arg(a, int);
          if (c == 'i' && i < 0) i = -i, PUTC('-');
          xtoa((unsigned)i, dv + 5);
          break;
        case 'l':  // 32 bit Long
        case 'n':  // 32 bit uNsigned loNg
          n = va_arg(a, long);
          if (c == 'l' && n < 0) n = -n, PUTC('-');
          xtoa((unsigned long)n, dv);
          break;
        case 'x':  // 16 bit heXadecimal
          i = va_arg(a, int);
          d = i >> 12;
          if (d > 0 || fill_zeros >= 4) puth(d);
          d = i >> 8;
          if (d > 0 || fill_zeros >= 3) puth(d);
          d = i >> 4;
          if (d > 0 || fill_zeros >= 2) puth(d);
          puth(i);
          break;
        case '0':
          c = *format++;
          fill_zeros = c - '0';
          goto parse_fmt_char;
        case 0:
          return 0;
        default:
          goto bad_fmt;
      }
    } else
    bad_fmt:
      PUTC(c);
  }
  va_end(a);

  return 0;  // TODO: return number of chars printed
}
