#include <chrt.h>
#include <chrt_calib.h>
#include <msp430.h>
#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdint.h>

void calibrate_charging_times() {
  for (int j = 0; j < 4; j++) {
    msp_printf("\nStarting charge time calibration for tier %x:\n", j);
    for (int i = 0; i < 10; i++) {
      chrt_calibrate_charging_time(j);
    }
  }
}

// void calibrate_discharging_times() {
//  for (int j = 0; j < 4; j++) {
//    msp_printf("\nStarting discharge time calibration for tier %x:\n", j);
//    for (int i = 0; i < 10; i++) {
//      chrt_calibrate_discharging_time(j);
//    }
//  }
//}

int main(void) {
  msp_watchdog_disable();

  // Initialize and configure pins.
  msp_gpio_init_all_ports();
  msp_gpio_unlock();

  // Change clock frequency to 8 MHz for MCLK and SMCLK.
  msp_clock_set_mclk(CLK_8_MHZ, SMCLK_8_MHZ);

  msp_uart_open();
  msp_printf("\n\nStarting discharge test \n\n");

  chrt_init(0, 0, 0, true);

  calibrate_charging_times();
  //  calibrate_discharging_times();

  while (1) {
    __delay_cycles(100);
  }
  return 0;
}
