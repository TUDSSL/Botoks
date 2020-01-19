#include <chrt.h>
#include <chrt_calib.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdint.h>
#include <tptime.h>

#define CLEAR_EXPIRATION 0

__nv uint16_t foo_data = 0xF00;

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

  /* Initialize TPTime. */
  tptime_init();

  /* Check for expiration of variable. */
  int8_t result = tptime_has_expired(&foo_data);
  if (result == TPTIME_OBJ_EXPIRED) {
    msp_printf("Pre-set: Variable 'foo_data' has expired.\n");
#if CLEAR_EXPIRATION
    tptime_clear_expiration(&foo_data);
#endif
  } else if (result == TPTIME_OBJ_NOT_FOUND) {
    msp_printf("Pre-set: Variable 'foo_data' not found.\n");
  }

  /* Print timestamp. */
  uint32_t timestamp = tptime_get_timestamp();
  msp_printf("System time: %l\n", timestamp);

  /* Set expiration for variable (expires in 50ms). */
  tptime_set_expiration(&foo_data, 50);

  /* Check for expiration of variable. */
  result = tptime_has_expired(&foo_data);
  if (result == TPTIME_OBJ_EXPIRED) {
    msp_printf("Post-set: Variable 'foo_data' has expired.\n");
#if CLEAR_EXPIRATION
    tptime_clear_expiration(&foo_data);
#endif
  } else if (result == TPTIME_OBJ_NOT_FOUND) {
    msp_printf("Post-set: Variable 'foo_data' not found.\n");
  }


  /* Reset the MCU simulating intermittency. */
  WDTCTL = 0xDEAD;

  return 0;
}
