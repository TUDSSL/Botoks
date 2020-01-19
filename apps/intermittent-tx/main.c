#include <chrt.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdint.h>
#include <tpsync.h>
#include <zl70550.h>

/* For hierarchical timekeepers */
#define TK_BND_MS 100
#define TK_BND_TICKS (uint16_t)(TK_BND_MS * 1000ul / US_PER_TICK)
__nv uint8_t curr_tk_id = 0;

/* For wakeup period */
uint16_t elapsed_time;
uint16_t mean_wakeup_period;

/* Packet */
typedef struct {
  uint16_t mean_wakeup_period;
  uint16_t seq_nr;
} packet_t;

/* TX buffer */
packet_t tx_pkt;
__nv uint16_t seq_nr = 0;

static inline uint16_t get_elapsed_time() {
  chrt_init(1, 0, 0, true);

  /* Retrieve time. */
  uint16_t resolution, t;
  chrt_sample_tier(1, &resolution, &t, false);

  chrt_charge_tier(1);
  chrt_start_discharge(1);

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

  /* Compute mean wakeup period. */
  mean_wakeup_period = get_avg_wakeup_period(elapsed_time);

#if TPSYNC_DELAY_TX_ENABLE
  /* Delay radio transmission to snap to multiple of base period. */
  delay_radio_transmission(elapsed_time);
#endif

  /* Initialize radio. */
  radio_init();

#if PROFILE
  msp_gpio_set(PRF_PORT, RADIO_PIN);
#endif

  /* Send packet. */
  tx_pkt.mean_wakeup_period = mean_wakeup_period;
  tx_pkt.seq_nr = seq_nr;
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
 * Comparator COMP_E ISR.
 */
__attribute__((interrupt(COMP_E_VECTOR))) void COMP_E_ISR(void) {
  /* Turn off comparator and disable interrupt. */
  CECTL1 &= ~CEON;
  CEINT &= ~CEIE;

  /* Disable general interrupts and exit LPM4. */
  __bic_SR_register_on_exit(GIE | LPM4_bits);
}

/**
 * Timer TA1 ISR.
 */
__attribute__((interrupt(TIMER1_A0_VECTOR))) void TIMER_TA1_ISR(void) {
  __bic_SR_register_on_exit(GIE | LPM1_bits);
}
