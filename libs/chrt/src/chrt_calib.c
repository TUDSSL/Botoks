#include <chrt_calib.h>
#include <math.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>

extern uint16_t wakeup_voltage;
extern uint16_t charge_times[N_TIERS];
extern uint16_t discharge_times[N_TIERS];

extern uint8_t chrt_complete_discharge(uint8_t tk_id);

void start_sleep_timer(uint32_t end) {
  uint16_t timer_overflows = end >> 16;

  while (timer_overflows) {
    // Start timer.
    TA0CCR0 = 0xFFFF;
    TA0CTL |= MC__UP | TACLR;

    // Wait for expiration and then stop timer.
    __bis_SR_register(LPM0_bits);
    TA0CTL |= TACLR;

    timer_overflows--;
  }

  // Start timer.
  TA0CCR0 = (uint16_t)(end & 0x0000FFFF);
  TA0CTL |= MC__UP | TACLR;

  // Wait for expiration and then stop timer.
  __bis_SR_register(LPM0_bits);
  TA0CTL |= TACLR;
}

void init_sleep_timer() {
  // Configure SMCLK  at 0.5 MHz.
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_0_5_MHZ);

  // Configure timer TA0:
  // - sourced by SMCLK (0.5 MHz)
  // - clock divided by 8 * 6 = 48
  //   => counting at most up to 6291 ms
  TA0CTL = TASSEL__SMCLK | ID__8;
  TA0EX0 = TAIDEX__6;
  TA0CCTL0 = CCIE;

  __enable_interrupt();
}

uint8_t chrt_calibrate_charging_time(uint8_t tk_id) {
  charge_times[tk_id] = 0;
  // charge_times[tk_id] = INITIAL_CHARGE_TIME;
  wakeup_voltage = 0;

  while (wakeup_voltage < 0xfff) {
    charge_times[tk_id]++;
    chrt_charge_tier(tk_id);
    chrt_start_discharge(tk_id);
    // get wakeup_voltage
    uint16_t resolution, time;
    chrt_sample_tier(tk_id, &resolution, &time, false);
  }
  msp_printf("Charging time: %u for range %u \n", charge_times[tk_id], tk_id);
  return TK_SUCCESS;
}

uint8_t chrt_calibrate_discharging_time(uint8_t tk_id) {
  discharge_times[tk_id] = 0;
  // charge_times[tk_id] = INITIAL_CHARGE_TIME;
  wakeup_voltage = 0xFF;

  while (wakeup_voltage > 0x00f) {
    discharge_times[tk_id]++;
    chrt_charge_tier(tk_id);
    chrt_complete_discharge(tk_id);
    // get wakeup_voltage
    uint16_t resolution, time;
    chrt_sample_tier(tk_id, &resolution, &time, false);
  }
  msp_printf("Discharging time: %u for range %u \n", discharge_times[tk_id],
             tk_id);
  return TK_SUCCESS;
}

uint8_t chrt_calibrate_accuracy(uint8_t tk_id, uint32_t start, uint32_t end,
                                uint16_t step) {
  uint32_t time_ms = start;

  init_sleep_timer();

  while (time_ms <= end) {
    uint32_t end = ((uint32_t)time_ms * 1000 / US_PER_TICK);

    chrt_charge_tier(tk_id);
    chrt_start_discharge(tk_id);

    start_sleep_timer(end);

    uint16_t time_meas, resolution;
    chrt_sample_tier(tk_id, &resolution, &time_meas, true);

    //    uint32_t time_meas_ms = (uint32_t)time_meas * resolution;
    uint32_t time_meas_ms = (uint32_t)time_meas;
    msp_printf("%l, %u, %l\n", time_meas_ms, resolution, time_ms / resolution);

    time_ms += step;
  }

  msp_printf("\n\n");
  return TK_SUCCESS;
}

uint8_t chrt_test_repeatability(uint8_t tk_id, uint32_t time_ms,
                                uint8_t repeats) {
  init_sleep_timer(time_ms);

  uint32_t end = ((uint32_t)time_ms * 1000 / US_PER_TICK);

  for (uint16_t i = 0; i < repeats; i++) {
    chrt_charge_tier(tk_id);
    chrt_start_discharge(tk_id);

    start_sleep_timer(end);

    uint16_t time_meas, resolution;
    chrt_sample_tier(tk_id, &resolution, &time_meas, true);

    //    uint32_t time_meas_ms = (uint32_t)time_meas * resolution;
    uint32_t time_meas_ms = (uint32_t)time_meas;
    msp_printf("%l, %u, %l\n", time_meas_ms, resolution, time_ms / resolution);
  }

  msp_printf("\n\n");
  return TK_SUCCESS;
}

uint8_t chrt_test_cascaded_repeatability(uint32_t time_ms, uint8_t repeats) {
  init_sleep_timer(time_ms);

  uint32_t end = ((uint32_t)time_ms * 1000 / US_PER_TICK);

  for (uint16_t i = 0; i < repeats; i++) {
    chrt_charge();

    start_sleep_timer(end);

    uint16_t time_meas, resolution;
    chrt_get_time(&resolution, &time_meas, false);

    uint32_t time_result_ms = (uint32_t)time_meas;
    msp_printf("%l, %u, %l\n", time_result_ms, resolution,
               time_ms / resolution);
  }

  msp_printf("\n\n");
  return TK_SUCCESS;
}

uint8_t chrt_calibrate_accuracy_cascaded(uint32_t start, uint32_t end,
                                         uint16_t step) {
  uint32_t time_ms = start;

  init_sleep_timer();

  while (time_ms <= end) {
    uint32_t end = ((uint32_t)time_ms * 1000 / US_PER_TICK);

    chrt_charge();

    start_sleep_timer(end);

    uint16_t time_meas, resolution;
    chrt_get_time(&resolution, &time_meas, false);

    uint32_t time_result_ms = (uint32_t)time_meas;
    msp_printf("%l, %u, %l\n", time_result_ms, resolution,
               time_ms / resolution);

    time_ms += step;
  }

  msp_printf("\n\n");
  return TK_SUCCESS;
}

__attribute__((interrupt(TIMER0_A0_VECTOR))) void TIMER0_A0_ISR(void) {
  __bic_SR_register_on_exit(LPM0_bits);
}
