#include <chrt.h>
#include <chrt_calib.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdint.h>

int main(void) {
  msp_watchdog_disable();

  /* Initialize and configure pins. */
  msp_gpio_init_all_ports();
  msp_gpio_unlock();

  /* Change clock frequency to 8 MHz for MCLK and SMCLK. */
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_8_MHZ);

  msp_uart_open();

  msp_gpio_dir_out(DBG_PORT, DEBUG_POWER_PIN);
  msp_gpio_spike(DBG_PORT, DEBUG_POWER_PIN);

  /* 10ms delay. */
  __delay_cycles(80000);

  /* Initialize Raw API. */
  chrt_init(0, N_TIERS - 1, N_TIERS - 1, true);

  /* Retrieve the time. (charging time not compensated). */
  uint16_t time_meas, resolution;
  chrt_get_time(&resolution, &time_meas, false);

  /* Charge the CHRT */
  chrt_charge();

  uint32_t time_meas_ms = (uint32_t)resolution * time_meas;
  msp_printf("Measured time: %l, Resolution: %u, Raw time: %u\n", time_meas_ms,
             resolution, time_meas);

  /* Reset the MCU simulating intermittency. */
  WDTCTL = 0xDEAD;

  return 0;
}
