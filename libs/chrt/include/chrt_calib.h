/*
 * chrt_calib.h
 *
 *  Created on: Nov 25, 2019
 *      Author: jasper
 */

#ifndef LIBS_CHRT_INCLUDE_CHRT_CALIB_H_
#define LIBS_CHRT_INCLUDE_CHRT_CALIB_H_

#include <chrt.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t chrt_test_repeatability(uint8_t tk_id, uint32_t time_ms,
                                uint8_t repeats);
uint8_t chrt_test_cascaded_repeatability(uint32_t time_ms, uint8_t repeats);
uint8_t chrt_calibrate_accuracy_cascaded(uint32_t start, uint32_t end,
                                         uint16_t step);

uint8_t chrt_calibrate_charging_time(uint8_t tk_id);
uint8_t chrt_calibrate_discharging_time(uint8_t tk_id);
uint8_t chrt_calibrate_accuracy(uint8_t tk_id, uint32_t start, uint32_t end,
                                uint16_t step);

#endif /* LIBS_CHRT_INCLUDE_CHRT_CALIB_H_ */
