#include <chrt.h>
#include <chrt_calib.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdint.h>

int main(void) {
  msp_watchdog_disable();

  // Initialize and configure pins.
  msp_gpio_init_all_ports();
  msp_gpio_unlock();

  // Change clock frequency to 8 MHz for MCLK and SMCLK.
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_8_MHZ);

  msp_uart_open();
  msp_printf("\n\nStarting Accuracy calibration \n\n");

  chrt_init(0, 0, 0, true);

#if TIER == 0
  msp_printf("Tier 0:\n");
  chrt_calibrate_accuracy(0, 1, 100, 1);
#elif TIER == 1
  msp_printf("Tier 1:\n");
  chrt_calibrate_accuracy(1, 10, 1000, 10);
#elif TIER == 2
  msp_printf("Tier 2:\n");
  chrt_calibrate_accuracy(2, 100, 10000, 100);
#elif TIER == 3
  msp_printf("Tier 3:\n");
  chrt_calibrate_accuracy(3, 1000, 9000, 1000);
  chrt_calibrate_accuracy(3, 10000, 100000, 5000);
#endif

  msp_printf("Done\n");

  while (1) {
    __delay_cycles(100);
  }

  return 0;
}
