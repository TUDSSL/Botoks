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
uint8_t prev_tk_id;

/* For wakeup period */
uint16_t elapsed_time;
__nv int16_t tx_mean_wakeup_period;

/* Packet */
typedef struct {
  uint16_t mean_wakeup_period;
  uint16_t seq_nr;
} packet_t;

/* RX buffer */
packet_t rx_pkt;

/* Timekeeper's error (= calibration granularity). */
__nv uint16_t error[N_TIERS];

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

  error[0] = 200 / US_PER_TICK;
  error[1] = 1000 / US_PER_TICK;

  /* Get time and recharge timekeeper. */
  elapsed_time = get_elapsed_time();

#if TPSYNC_ENABLE
  /* Delay radio listening. */
  delay_radio_listening(elapsed_time, tx_mean_wakeup_period, 0);
#endif

  /* Initialize radio. */
  radio_init();

#if TPSYNC_ENABLE
  /* Start tracking sync error. */
  track_sync_error_start();

#if PROFILE_RX_ERROR
  msp_gpio_set(DBG_PORT, DEBUG_B);
#endif

#endif

#if PROFILE
  msp_gpio_set(PRF_PORT, RADIO_PIN);
#endif

  /* Listen for incoming packet. */
  zl70550_packet_receive((uint8_t*)&rx_pkt, false);
  tx_mean_wakeup_period = rx_pkt.mean_wakeup_period;

#if TPSYNC_ENABLE

#if PROFILE_RX_ERROR
  msp_gpio_clear(DBG_PORT, DEBUG_B);
#endif

  /* Stop tracking sync error. */
  track_sync_error_stop();
#endif

#if PROFILE
  msp_gpio_spike(PRF_PORT, RX_PKT_PIN);
#endif

#if PROFILE
  msp_gpio_clear(PRF_PORT, RADIO_PIN);
#endif

  /* Waste remaining energy for shorter wake-up period (quicker testing). */
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
