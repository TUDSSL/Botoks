#include <chrt.h>
#include <math.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>

/** Non-volatile timekeeper lookup table. Relates the ADC voltage to time*/
extern uint16_t log_lut[N_TIERS][TABLE_LEN];

/** Scale per tier to reach ms*/
__nv uint32_t scale_per_tier[N_TIERS];
/** Charge time in loop cycles to charge a tier to ADC VRef*/
__nv uint16_t charge_times[N_TIERS];
/** Time in loop cycles required to completely discharge a given tier*/
__nv uint16_t discharge_times[N_TIERS];

/** Is this the first executing since programming?*/
__nv uint8_t n_init;
/** True if cascade is active.*/
__nv uint8_t cascade_active;
/** Variables tracking ideal tier given te reboot rate and the margin
  of charging multiple tier around this active tier.*/
__nv uint8_t target_tier, tier_margin_up, tier_margin_down;
/** Are we operating in non cascaded mode? */
__nv bool fixed_tier;

/** Scaled total time of each tier. Must be in the scale of the next tier. */
const uint8_t total_tier_time[N_TIERS - 1] = {
    TIER_TOTAL_TIME_0, TIER_TOTAL_TIME_1, TIER_TOTAL_TIME_2};

const uint8_t tier_charge_time_ms[N_TIERS] = {
    TIER_CHARGE_TIME_0_MS, TIER_CHARGE_TIME_1_MS, TIER_CHARGE_TIME_2_MS,
    TIER_CHARGE_TIME_3_MS};

/** ADC value on wake-up (for timekeeping)*/
uint16_t wakeup_voltage;

/** High if its the first boot after programming */
bool first_boot = false;

uint8_t chrt_start_discharge(uint8_t tk_id);
uint8_t chrt_stop_discharge(uint8_t tk_id);
uint8_t chrt_populate_table();

uint8_t chrt_init(uint8_t start_tier, uint8_t KR, uint8_t KL, bool fix_tiers) {
  msp_gpio_dir_in(ADC_PORT, ADC_PIN_0);
  msp_gpio_dir_in(ADC_PORT, ADC_PIN_1);
  msp_gpio_dir_in(ADC_PORT, ADC_PIN_2);
  msp_gpio_dir_in(ADC_PORT, ADC_PIN_3);

  msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_0);
  msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_1);
  msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_2);
  msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_3);

  msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_0);
  msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_1);
  msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_2);
  msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_3);
  msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
  msp_gpio_clear(CAS_EN_ADC_PORT, CAS_EN_ADC_EN);
  msp_gpio_clear(CAS_LDO_PORT, CAS_LDO_EN);

  // Mod to prevent backfeeding the MCU.
#if HWREV == 3
  msp_gpio_dir_out(CAS_EN_ADC_PORT, CAS_EN_ADC_EN);
#endif

  if (n_init) {
    fixed_tier = fix_tiers;
    first_boot = true;
    chrt_populate_table();

    if (KR >= N_TIERS || KL >= N_TIERS || start_tier >= N_TIERS) {
      return TK_ERROR;
    }

    target_tier = start_tier;

    tier_margin_up = KR;
    tier_margin_down = KL;

    if (!KR && !KL) {
      cascade_active = false;
    } else {
      cascade_active = true;
    }
    n_init = 0;
  }

  // enable/disable Cascade
  if (cascade_active) {
    msp_gpio_set(CAS_PORT, CAS_EN_PIN);
  } else {
    msp_gpio_clear(CAS_PORT, CAS_EN_PIN);
  }

  msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
  __delay_cycles(100);
  msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);

  return TK_SUCCESS;
}

uint8_t chrt_complete_discharge(uint8_t tk_id) {
  msp_gpio_set(CAS_EN_ADC_PORT, CAS_EN_ADC_EN);
  // Fully discharge the timekeeper
  switch (tk_id) {
    case 0:
      msp_gpio_dir_out(ADC_PORT, ADC_PIN_0);
      msp_gpio_clear(ADC_PORT, ADC_PIN_0);
      for (uint16_t i = 0; i < discharge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_dir_in(ADC_PORT, ADC_PIN_0);
      break;
    case 1:
      msp_gpio_dir_out(ADC_PORT, ADC_PIN_1);
      msp_gpio_clear(ADC_PORT, ADC_PIN_1);
      for (uint16_t i = 0; i < discharge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_dir_in(ADC_PORT, ADC_PIN_1);
      break;
    case 2:
      msp_gpio_dir_out(ADC_PORT, ADC_PIN_2);
      msp_gpio_clear(ADC_PORT, ADC_PIN_2);
      for (uint16_t i = 0; i < discharge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_dir_in(ADC_PORT, ADC_PIN_2);
      break;
    case 3:
      msp_gpio_dir_out(ADC_PORT, ADC_PIN_3);
      msp_gpio_clear(ADC_PORT, ADC_PIN_3);
      for (uint16_t i = 0; i < discharge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_dir_in(ADC_PORT, ADC_PIN_3);
      break;
    default:
      return TK_UNKNOWN_ID;
  }
  msp_gpio_clear(CAS_EN_ADC_PORT, CAS_EN_ADC_EN);
  return TK_SUCCESS;
}

uint8_t chrt_charge_tier(uint8_t tk_id) {
  // Discharge the timekeeper
  chrt_complete_discharge(tk_id);
  // Enable the LDO.
  msp_gpio_set(CAS_LDO_PORT, CAS_LDO_EN);

  // Charge the timekeeper
  switch (tk_id) {
    case 0:
      msp_gpio_set(CHARGE_PORT, CHARGE_PIN_0);
      for (uint16_t i = 0; i < charge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_0);
      break;
    case 1:
      msp_gpio_set(CHARGE_PORT, CHARGE_PIN_1);
      for (uint16_t i = 0; i < charge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_1);
      break;
    case 2:
      msp_gpio_set(CHARGE_PORT, CHARGE_PIN_2);
      for (uint16_t i = 0; i < charge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_2);
      break;
    case 3:
      msp_gpio_set(CHARGE_PORT, CHARGE_PIN_3);
      for (uint16_t i = 0; i < charge_times[tk_id]; i++) {
        __delay_cycles(DELAY_CYCLES);
      }
      msp_gpio_clear(CHARGE_PORT, CHARGE_PIN_3);
      break;
    default:
      return TK_UNKNOWN_ID;
  }

  msp_gpio_clear(CAS_LDO_PORT, CAS_LDO_EN);

  return TK_SUCCESS;
}

uint8_t chrt_start_discharge(uint8_t tk_id) {
  switch (tk_id) {
    case 0:
      msp_gpio_clear(TIME_EN_PORT, TIME_EN_PIN_0);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      __delay_cycles(100);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    case 1:
      msp_gpio_clear(TIME_EN_PORT, TIME_EN_PIN_1);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      __delay_cycles(100);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    case 2:
      msp_gpio_clear(TIME_EN_PORT, TIME_EN_PIN_2);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      __delay_cycles(100);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    case 3:
      msp_gpio_clear(TIME_EN_PORT, TIME_EN_PIN_3);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      __delay_cycles(100);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    default:
      return TK_UNKNOWN_ID;
  }

  return TK_SUCCESS;
}

uint8_t chrt_stop_discharge(uint8_t tk_id) {
  switch (tk_id) {
    case 0:
      msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_0);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    case 1:
      msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_1);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    case 2:
      msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_2);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    case 3:
      msp_gpio_set(TIME_EN_PORT, TIME_EN_PIN_3);
      msp_gpio_set(CAS_PORT, CAS_CLK_PIN);
      msp_gpio_clear(CAS_PORT, CAS_CLK_PIN);
      break;
    default:
      return TK_UNKNOWN_ID;
  }
  return TK_SUCCESS;
}

uint8_t chrt_sample_tier(uint8_t tk_id, uint16_t* resolution, uint16_t* time,
                         bool debug) {
  msp_gpio_set(CAS_EN_ADC_PORT, CAS_EN_ADC_EN);
  // Configure internal REFerence module at 1.2V for ADC.
  while (REFCTL0 & REFGENBUSY)
    ;
  REFCTL0 |= REFVSEL_0 | REFON;

  // Configure ADC12 (for single conversion):
  // - 16 ADCCLK sample-and-hold time
  // - sourced by SMCLK
  // - 12-bit resolution
  // - results stored in MEM0 register
  // - VR+ = VeREF+, VR- = AVSS
  // - channel A2/A3/A4/A5 (corresponding to pin 1.2/1.3/1.4/1.5)
  ADC12CTL0 |= ADC12SHT0_8;
  ADC12CTL1 |= ADC12SSEL_3 | ADC12SHP;
  ADC12CTL2 |= ADC12RES__12BIT;
  ADC12CTL3 |= ADC12CSTARTADD__ADC12MEM0;
  ADC12MCTL0 |= ADC12VRSEL_1;
  switch (tk_id) {
    case 0:
      *resolution = TIER_0_RESOLUTION;
      ADC12MCTL0 |= ADC_CHANNEL_0;
      break;
    case 1:
      *resolution = TIER_1_RESOLUTION;
      ADC12MCTL0 |= ADC_CHANNEL_1;
      break;
    case 2:
      *resolution = TIER_2_RESOLUTION;
      ADC12MCTL0 |= ADC_CHANNEL_2;
      break;
    case 3:
      *resolution = TIER_3_RESOLUTION;
      ADC12MCTL0 |= ADC_CHANNEL_3;
      break;
    default:
      // TODO: signal error
      return 0;
  }
  ADC12IER0 |= ADC12IE0;

  // Wait for reference to settle.
  while (!(REFCTL0 & REFGENRDY))
    ;

  // Turn on ADC.
  ADC12CTL0 |= ADC12ON;

  // Enable general interrupts.
  __enable_interrupt();

  // Trigger ADC and wait for sample.a
  ADC12CTL0 |= ADC12ENC | ADC12SC;
  __bis_SR_register(LPM1_bits);

  // Turn off ADC and REF and disable all interrupts.
  ADC12CTL0 &= ~ADC12ENC;
  ADC12MCTL0 &= ~ADC12INCH;
  ADC12CTL0 &= ~ADC12ON;
  REFCTL0 &= ~REFON;
  ADC12IER0 &= ~ADC12IE0;
  //   __disable_interrupt();

  if (debug) {
    msp_printf("%x, ", wakeup_voltage);
  }
  *time = log_lut[tk_id][wakeup_voltage];

  msp_gpio_clear(CAS_EN_ADC_PORT, CAS_EN_ADC_EN);
  return TK_SUCCESS;
}

uint8_t chrt_get_time(uint16_t* resolution, uint16_t* time, bool cmp_chr_time) {
  if (!resolution || !time) {
    return TK_ERROR;
  }

  uint8_t finalTier = (target_tier + tier_margin_up > (N_TIERS - 1))
                          ? (N_TIERS - 1)
                          : target_tier + tier_margin_up;
  uint8_t startTier =
      (tier_margin_down > target_tier) ? 0 : target_tier - tier_margin_down;
  uint8_t tier_to_sample = startTier;
  uint8_t nextTier;
  uint16_t time_wakeup;
  bool sampled_time = false;

  for (; tier_to_sample <= finalTier; tier_to_sample++) {
    chrt_sample_tier(tier_to_sample, resolution, &time_wakeup, false);
    if (!cascade_active) {
      // non cascade time from single tier
      *time = time_wakeup;
      nextTier = tier_to_sample;
      sampled_time = true;
      break;
    }
    if (wakeup_voltage >= CASCADE_TRIGGER_THRESHOLD) {
      // retrieved time from one of the tiers
      *time = time_wakeup;
      if (tier_to_sample > 0) {
        *time += total_tier_time[tier_to_sample - 1];
      }
      nextTier = tier_to_sample;
      sampled_time = true;
      break;
    }
  }

  if (cmp_chr_time) {
    if (*resolution == TIER_0_RESOLUTION) {
      // 1 ms scale so directly add the charging times.
      for (uint8_t i = startTier; i <= finalTier; ++i) {
        *time += tier_charge_time_ms[i];
      }
    } else if (*resolution == TIER_1_RESOLUTION) {
      // 10 ms scale so only if fully charged (~5ms)
      // add one as compensation.
      if ((finalTier - startTier) == (N_TIERS - 1)) {
        *time += 1;
      }
    }
  }

  if (sampled_time) {
    // changing of the ideal timing tier (range) to use
    if (!first_boot && !fixed_tier) {
      target_tier = nextTier;
      if ((*time > TIER_CHANGE_LEVEL_HIGH) && (target_tier < (N_TIERS - 1))) {
        target_tier++;
      } else if ((*time < TIER_CHANGE_LEVEL_LOW) && (target_tier > 0)) {
        target_tier--;
      }
    }
    return TK_SUCCESS;
  } else {
    // attempt to recover the ideal timing tier (range) to use
    if (wakeup_voltage < CASCADE_TRIGGER_THRESHOLD && !first_boot &&
        !fixed_tier) {
      target_tier++;
    }
    return TK_ERROR;
  }
}

uint8_t chrt_populate_table() {
  charge_times[0] = TIER_CHARGE_TIME_0;
  charge_times[1] = TIER_CHARGE_TIME_1;
  charge_times[2] = TIER_CHARGE_TIME_2;
  charge_times[3] = TIER_CHARGE_TIME_3;

  discharge_times[0] = TIER_DISCHARGE_TIME_0;
  discharge_times[1] = TIER_DISCHARGE_TIME_1;
  discharge_times[2] = TIER_DISCHARGE_TIME_2;
  discharge_times[3] = TIER_DISCHARGE_TIME_3;

  return TK_SUCCESS;
}

uint8_t chrt_charge() {
  uint8_t lowerMargin =
      (tier_margin_down > target_tier) ? 0 : target_tier - tier_margin_down;
  uint8_t upperMargin = ((target_tier + tier_margin_up) > (N_TIERS - 1))
                            ? (N_TIERS - 1)
                            : (target_tier + tier_margin_up);

  for (int8_t i = upperMargin; i >= lowerMargin; i--) {
    chrt_charge_tier(i);
  }

  if (cascade_active) {
    chrt_start_discharge(0);
  } else {
    chrt_start_discharge(lowerMargin);
  }

  return TK_SUCCESS;
}

__attribute__((interrupt(ADC12_B_VECTOR))) void ADC12_ISR(void) {
  wakeup_voltage = ADC12MEM0;
  __bic_SR_register_on_exit(LPM0_bits);
}
