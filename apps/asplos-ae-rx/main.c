#include <stdint.h>
#include <string.h>

#include <msp430.h>
#include <mspbase.h>
#include <platform.h>
#include <mspprintf.h>
#include <zl70550.h>

/* Radio packet. */
typedef struct {
  uint16_t period;
  uint16_t tier;
  uint16_t sq_number;
} packet_t;

/* RX buffer. */
uint8_t rx_buf[256];

uint16_t timer_overflows;

static void radio_init() {
  /* Power the radio. */
  msp_gpio_set(zl70550_POWER_PORT, zl70550_POWER_PIN);
  // TODO: optimize waiting time
  __delay_cycles(8000);

  zl70550_init();

  zl70550_restore_state();

  zl70550_set_frequency();

  zl70550_calibrate();
}

static void timer_init() {
  timer_overflows = 0;

  /* Configure SMCLK at 0.25 MHz. */
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_8_MHZ);

  /* Configure timer TA0: */
  /* - sourced by SMCLK (8 MHz) */
  /* - clock divided by 8 * 8 = 64 */
  /* => 1 tick is 0.008 ms, counting at most up to ~500 ms */
  TA0CTL = TASSEL__SMCLK | ID__8;
  TA0EX0 = TAIDEX__8;
  TA0CCTL0 |= CCIE;

  __enable_interrupt();

  /* Start timer TA0. */
  TA0CCR0 = 0xFFFF;
  TA0CTL |= MC__UP | TACLR;
}

int main(void) {
  msp_watchdog_disable();

  /* Initialize and configure pins. */
  msp_gpio_init_all_ports();
  msp_gpio_unlock();

  /* Initialize UART. */
  msp_uart_open();

  /* Change clock frequency to 8 MHz for MCLK and SMCLK. */
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_8_MHZ);

  /* Initialize radio. */
  radio_init();

  msp_printf("Init complete.\n");

  /* Initialize timer (used to capture time between two received packets). */
  timer_init();

  while (1) {

    /* Receive packet from intermittently-powered Botoks. */
    zl70550_packet_receive((uint8_t*)rx_buf, false);

    /* Calculate current time in ms (each timer tick is 0.008 ms). */
    uint16_t current_timestamp = (((uint32_t)timer_overflows << 16) + TA0R) / 128;

    /* Reset timer. */
    TA0CTL &= ~MC__UP;
    TA0CTL |= TACLR;
    TA0CTL |= MC__UP;

    timer_overflows = 0;

    /* Print elapsed time (ground truth VS Botoks). */
    msp_printf("Elapsed time [ms]. Ground truth: %u, Botoks: %u\n",
               current_timestamp, rx_buf[0] | rx_buf[1] << 8);
  }
}

__attribute__((interrupt(TIMER0_A0_VECTOR))) void TIMER0_A0_ISR(void) {
  timer_overflows++;
}
