#include <chrt.h>
#include <mspbase.h>
#include <platform.h>
#include <tpsync.h>

/* Constants based on configuration parameters. */
#define SYNC_SLACK (SYNC_SLACK_MS * 1000 / 12)

/* For sync recovery (experimentally tuned). */
#define MAX_RECOVERY_PERIODS 5u
#define RECOVERY_DELAY_DECREMENT 4u

/* Radio transmission constants (used to calculate how many ticks is takes to
 * send the preamble). */
#define PREAMBLE_BYTES 4 + 10 + 3
#define SYNC_BYTES 3
#define DATA_RATE 200000

/* Microseconds in a sync timer tick (one eighth of calibration timer tick). */
#define US_PER_SYNC_TICK (US_PER_TICK / 8)

/* Timer ticks spent on sending preamble and sync word. */
#define PREAMBLE_TIME_US (1e6 * 8 * (PREAMBLE_BYTES + SYNC_BYTES) / DATA_RATE)
#define PREAMBLE_TICKS ((uint16_t)PREAMBLE_TIME_US / US_PER_SYNC_TICK)

/* For regulated transmission: base period (expressed in timer ticks) is ceiled
 * to a power of 2 for easier calculations. */
#define __ceil_u16_to_pow_2(x)                                                \
  ((((x - 1) | ((x - 1) >> 1)) | (((x - 1) | ((x - 1) >> 1)) >> 2) |          \
    (((x - 1) | ((x - 1) >> 1)) | (((x - 1) | ((x - 1) >> 1)) >> 2) >> 4) |   \
    (((x - 1) | ((x - 1) >> 1)) | (((x - 1) | ((x - 1) >> 1)) >> 2) |         \
     (((x - 1) | ((x - 1) >> 1)) | (((x - 1) | ((x - 1) >> 1)) >> 2) >> 4) >> \
         8)) +                                                                \
   1)
#define TX_BASE_PERIOD_MS 25u
#define TX_BASE_PERIOD_TICKS \
  ((uint16_t)(TX_BASE_PERIOD_MS * 1000l / US_PER_TICK))
#define TX_BASE_PERIOD_POW_2 __ceil_u16_to_pow_2(TX_BASE_PERIOD_TICKS)

/* Clock configuration backup. */
uint16_t dcofsel_bak, divs_bak;

/* For wake-up period sync. */
__nv uint16_t rx_time_error;              // RX side
__nv uint8_t rx_successful = 0;           // RX side
__nv uint8_t sync_recovery = 0;           // RX side
__nv uint16_t wakeup_period[AVG_WINDOW];  // TX side
__nv int16_t delta_wakeup_period;         // RX and TX side

uint16_t get_avg_wakeup_period(uint16_t last_period) {
  uint32_t wakeup_period_sum = 0;
  for (uint8_t i = AVG_WINDOW - 1; i > 0; i--) {
    wakeup_period[i] = wakeup_period[i - 1];
    wakeup_period_sum += wakeup_period[i];
  }
  wakeup_period[0] = last_period;
  wakeup_period_sum += last_period;
  return (uint16_t)(wakeup_period_sum / AVG_WINDOW);
}

int delay_radio_transmission(uint16_t tx_wakeup_period) {
  /* Compute delay period based on TX wake-up time. */
  uint16_t added_delay =
      TX_BASE_PERIOD_POW_2 - (tx_wakeup_period & (TX_BASE_PERIOD_POW_2 - 1));
  delta_wakeup_period =
      (delta_wakeup_period + added_delay) & (TX_BASE_PERIOD_POW_2 - 1);

  /* Translate delay into timer ticks: one clock tick equals one eighth of a
   * calibration timer tick. */
  uint16_t ccr = (uint16_t)(delta_wakeup_period)*8;
  if (ccr == 0) {
    return TPSYNC_NULL_DELAY;
  }

  /* Save clock configuration and configure SMCLK at 0.5 MHz. */
  dcofsel_bak = CSCTL1 & DCOFSEL;
  divs_bak = CSCTL3 & DIVS;
  CSCTL0_H = CSKEY_H;
  CSCTL3 &= ~DIVS;
  switch (dcofsel_bak) {
    case DCOFSEL_0:  // DCO at 1 MHz, divide by 2
      CSCTL3 |= DIVS__2;
      break;
    case DCOFSEL_3:  // DCO at 4 MHz, divide by 8
      CSCTL3 |= DIVS__8;
      break;
    default:  // Unsupported DCO freq, set to 8 MHz
      CSCTL1 &= ~DCOFSEL;
      CSCTL1 |= DCOFSEL_6;
      /* fall-through */
    case DCOFSEL_6:  // DCO at 8 MHz, divide by 16
      CSCTL3 |= DIVS__16;
      break;
  }
  CSCTL0_H = 0;

  /* Configure timer TA1: */
  /* - sourced by SMCLK (0.5 MHz) */
  /* - clock divided by 1 * 6 = 6 (1 tick = 12 us, one eighth of a calibration
   *   timer tick) */
  /* => delay up to 786 ms */
  TA1CTL = TASSEL__SMCLK | ID__1;
  TA1EX0 = TAIDEX__6;
  TA1CCTL0 = CCIE;

#if PROFILE
  msp_gpio_set(PRF_PORT, DELAY_PIN);
#endif

  /* Start timer and enter LPM1. */
  TA1CCR0 = ccr;
  TA1CTL |= MC__UP | TACLR;
  __bis_SR_register(GIE | LPM1_bits);
  TA1CTL = TACLR;
  TA1CCTL0 = 0;

  /* Restore initial clock settings. */
  CSCTL0_H = CSKEY_H;
  CSCTL1 &= ~DCOFSEL;
  CSCTL1 |= dcofsel_bak;
  CSCTL3 &= ~DIVS;
  CSCTL3 |= divs_bak;
  CSCTL0_H = 0;

#if PROFILE
  msp_gpio_clear(PRF_PORT, DELAY_PIN);
#endif

  return TPSYNC_SUCCESS;
}

int delay_radio_listening(uint16_t rx_wakeup_period, uint16_t tx_wakeup_period,
                          uint16_t error) {
  /* Return in case of error on TX wake-up period. */
  if (tx_wakeup_period == 0) {
    return TPSYNC_NULL_TX_WUP;
  }

  msp_gpio_spike(PRF_PORT, DELAY_PIN);

  /* Update delay period based on TX and RX wake-up time. */
  if (rx_successful) {
    rx_successful = 0;
    sync_recovery = MAX_RECOVERY_PERIODS;
    if (rx_wakeup_period < INFINITE_TIME) {
      delta_wakeup_period =
          delta_wakeup_period + tx_wakeup_period - rx_wakeup_period;
    }
  } else {
    /* Invalidate sync error. */
    rx_time_error = 0;
    /* Adjust delay period in case sync was just lost. */
    if (sync_recovery) {
      sync_recovery--;
      error = error << 1;  // account for RX and TX timekeeping error
      if (delta_wakeup_period > error) {
        delta_wakeup_period -= error;
      } else {
        delta_wakeup_period = 0;
        sync_recovery = 0;
        return TPSYNC_NULL_DELAY;
      }
    } else {
      delta_wakeup_period = 0;
      return TPSYNC_NO_RX_SUCCESS;
    }
  }

  msp_gpio_spike(PRF_PORT, DELAY_PIN);

  /* Increment delay if negative (results in skipping some packets). */
  while (delta_wakeup_period < 0) {
    delta_wakeup_period += tx_wakeup_period;
  }

  /* Decrement delay if larger than needed. */
  while (delta_wakeup_period > tx_wakeup_period) {
    delta_wakeup_period -= tx_wakeup_period;
  }

  /* Translate delay into timer ticks: one clock tick equals one eighth of a
   * calibration timer tick. */
  uint16_t ccr = (uint16_t)delta_wakeup_period * 8;

  /* Apply error correction. */
  ccr += rx_time_error;

  /* Allow some sync slack. */
  if (ccr > SYNC_SLACK) {
    ccr -= SYNC_SLACK;
  } else {
    return TPSYNC_NULL_DELAY;
  }

  msp_gpio_spike(PRF_PORT, DELAY_PIN);

  /* Update delay after error correction. */
  delta_wakeup_period = ccr / 8;

  /* Save clock configuration and configure SMCLK at 0.5 MHz. */
  dcofsel_bak = CSCTL1 & DCOFSEL;
  divs_bak = CSCTL3 & DIVS;
  CSCTL0_H = CSKEY_H;
  CSCTL3 &= ~DIVS;
  switch (dcofsel_bak) {
    case DCOFSEL_0:  // DCO at 1 MHz, divide by 2
      CSCTL3 |= DIVS__2;
      break;
    case DCOFSEL_3:  // DCO at 4 MHz, divide by 8
      CSCTL3 |= DIVS__8;
      break;
    default:  // Unsupported DCO freq, set to 8 MHz
      CSCTL1 &= ~DCOFSEL;
      CSCTL1 |= DCOFSEL_6;
      /* fall-through */
    case DCOFSEL_6:  // DCO at 8 MHz, divide by 16
      CSCTL3 |= DIVS__16;
      break;
  }
  CSCTL0_H = 0;

  /* Configure timer TA1: */
  /* - sourced by SMCLK (0.5 MHz) */
  /* - clock divided by 1 * 6 = 6 (1 tick = 12 us, one eighth of a calibration
   *   timer tick) */
  /* => delay up to 786 ms */
  TA1CTL = TASSEL__SMCLK | ID__1;
  TA1EX0 = TAIDEX__6;
  TA1CCTL0 = CCIE;

#if PROFILE
  msp_gpio_set(PRF_PORT, DELAY_PIN);
#endif

  /* Start timer and enter LPM1. */
  TA1CCR0 = ccr;
  TA1CTL |= MC__UP | TACLR;
  __bis_SR_register(GIE | LPM1_bits);
  TA1CTL = TACLR;
  TA1CCTL0 = 0;

  /* Restore initial clock settings. */
  CSCTL0_H = CSKEY_H;
  CSCTL1 &= ~DCOFSEL;
  CSCTL1 |= dcofsel_bak;
  CSCTL3 &= ~DIVS;
  CSCTL3 |= divs_bak;
  CSCTL0_H = 0;

#if PROFILE
  msp_gpio_clear(PRF_PORT, DELAY_PIN);
#endif

  return TPSYNC_SUCCESS;
}

void track_sync_error_start(void) {
  /* Configure timer TA1: */
  /* - sourced by SMCLK (8 MHz) */
  /* - clock divided by 8 * 6 = 48 (1 tick = 6 us, one eighth of a calibration
   *   timer tick) */
  /* => counting at most up to 393 ms */
  /* - capture mode */
  TA1CTL = TASSEL__SMCLK | ID__8;
  TA1EX0 = TAIDEX__6;
  TA1CCTL1 = CAP | SCS | CCIS1 | CM_3;

  /* Start timer to track synchronization time error. */
  TA1CTL |= MC__CONTINUOUS | TACLR;
}

void track_sync_error_stop(void) {
  /* Get timer value for sync error, then stop timer. */
  TA1CCTL1 ^= CCIS0;
  while (!(TA1CCTL1 & CCIFG))
    ;
  rx_time_error = TA1CCR1 >> 1;
  TA1CTL &= ~MC;

  /* Flag successful packet reception. */
  rx_successful = 1;

  /* Adjust time synchronization error. Due to the interrupt capability of the
   * radio, preamble and sync word are included in the tracked error, so we
   * remove those. In addition, scale error by proportional coefficient. */
  if (rx_time_error > PREAMBLE_TICKS) {
    rx_time_error = (rx_time_error - PREAMBLE_TICKS) >> ERROR_CORR_COEFF;
  } else {
    rx_time_error = 0;
  }
}
