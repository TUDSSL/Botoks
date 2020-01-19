#include <chrt.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdint.h>
#include <tpsync.h>
#include <zl70550.h>

/* For wakeup period. */
uint16_t elapsed_time;
uint16_t tier;
uint8_t send;
uint16_t mean_wakeup_period;

/* Packet. */
typedef struct {
  uint16_t period;
  uint16_t tier;
  uint16_t sq_number;
} packet_t;

/* TX buffer. */
packet_t tx_pkt;
__nv uint16_t seq_nr;

static inline uint16_t get_elapsed_time() {
  chrt_init(0, N_TIERS - 1, N_TIERS - 1, false);

  // Retrieve time.
  uint16_t resolution, t;
  chrt_get_time(&resolution, &t, true);

  switch (resolution) {
    case 1:
      tier = 0;
      break;
    case 10:
      tier = 1;
      break;
    case 100:
      tier = 2;
      break;
    case 1000:
      tier = 3;
      break;
    default:
      break;
  }

  chrt_charge();

  /* may overflow after 65 secs */
  return t * resolution;
}

static inline void radio_init() {
  /* Power the radio. */
  msp_gpio_set(zl70550_POWER_PORT, zl70550_POWER_PIN);
  // TODO: optimize waiting time
  __delay_cycles(8000);

  zl70550_init();

  zl70550_restore_state();

  zl70550_set_frequency();

  zl70550_calibrate();
}

int main(void) {
  msp_watchdog_disable();

#if PROFILE
  msp_gpio_clear(PRF_PORT, DELAY_PIN);
  msp_gpio_dir_out(PRF_PORT, DELAY_PIN);
  msp_gpio_clear(PRF_PORT, RADIO_PIN);
  msp_gpio_dir_out(PRF_PORT, RADIO_PIN);
  msp_gpio_clear(PRF_PORT, RX_PKT_PIN);
  msp_gpio_dir_out(PRF_PORT, RX_PKT_PIN);

  msp_gpio_clear(DBG_PORT, DEBUG_B);
  msp_gpio_dir_out(DBG_PORT, DEBUG_B);
  msp_gpio_clear(DBG_PWR_PORT, DEBUG_POWER_PIN);
  msp_gpio_dir_out(DBG_PWR_PORT, DEBUG_POWER_PIN);

  msp_gpio_clear(PRF_PORT, RADIO_PIN);
#endif

  /* Initialize and configure pins. */
  msp_gpio_init_all_ports();
  msp_gpio_unlock();

  msp_uart_open();

#if PROFILE
  msp_gpio_set(DBG_PWR_PORT, DEBUG_POWER_PIN);
#endif

  /* Change clock frequency to 8 MHz for MCLK and SMCLK. */
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_8_MHZ);

  /* Get time and recharge timekeeper. */
  elapsed_time = get_elapsed_time();

  /* Initialize radio. */
  radio_init();

#if PROFILE
  msp_gpio_set(PRF_PORT, RADIO_PIN);
#endif

  /* Send packet. */
  tx_pkt.period = elapsed_time;
  tx_pkt.tier = tier;
  tx_pkt.sq_number = seq_nr;

  zl70550_packet_transmit((uint8_t*)&tx_pkt, sizeof(tx_pkt));
  seq_nr++;

#if PROFILE
  msp_gpio_clear(PRF_PORT, RADIO_PIN);
#endif

  /* Waste remaining energy for shorter wake-up period (quicker testing). */
  packet_t rx_pkt;
  zl70550_packet_receive((uint8_t*)&rx_pkt, true);

  while (1)
    ;

  return 0;
}

/**
 * Timer TA1 ISR.
 */
__attribute__((interrupt(TIMER1_A0_VECTOR))) void TIMER_TA1_ISR(void) {
  __bic_SR_register_on_exit(GIE | LPM1_bits);
}
