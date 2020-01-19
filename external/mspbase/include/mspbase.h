#ifndef INCLUDE_MSPBASE_H
#define INCLUDE_MSPBASE_H

#include <builtins.h>
#include <clock.h>
#include <gpio.h>
#include <msp430.h>
#include <watchdog.h>

/**
 * Send the MCU into an infinite loop.
 *
 * To escape, manually modify the program counter as follows:
 * PC <- PC + 2
 */
#define msp_black_hole() __asm__ volatile("DECD.W\tPC\n")

/**
 * Non-volatile section in linker script.
 */
#define __nv __attribute__((section(".nv_vars")))

#endif /* INCLUDE_MSPBASE_H */
