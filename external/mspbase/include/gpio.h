#ifndef INCLUDE_MSPBASE_GPIO_H
#define INCLUDE_MSPBASE_GPIO_H

/**
 * Internal macros
 */
#define __gpio_dir_in(port, pin) P##port##DIR &= ~BIT##pin
#define __gpio_dir_out(port, pin) P##port##DIR |= BIT##pin
#define __gpio_sel_0(port, pin) P##port##SEL0 |= BIT##pin
#define __gpio_sel_1(port, pin) P##port##SEL1 |= BIT##pin
#define __gpio_set(port, pin) P##port##OUT |= BIT##pin
#define __gpio_clear(port, pin) P##port##OUT &= ~BIT##pin
#define __gpio_toggle(port, pin) P##port##OUT ^= BIT##pin
#define __gpio_read(port, pin) P##port##IN& BIT##pin

/**
 * Disable the GPIO power-on default high-impedance mode to activate
 * previously configured port settings.
 */
#define msp_gpio_unlock() PM5CTL0 &= ~LOCKLPM5

/**
 * Initialize all GPIO ports for optimized power consumption.
     P1OUT = 0; P1DIR = 0xFF; \
 */
#define msp_gpio_init_all_ports() \
  P2OUT = 0;                      \
  P2DIR = 0xFF;                   \
  P3OUT = 0;                      \
  P3DIR = 0xFF;                   \
  P4OUT = 0;                      \
  P4DIR = 0xFF;                   \
  P5OUT = 0;                      \
  P5DIR = 0xFF;                   \
  P6OUT = 0;                      \
  P6DIR = 0xFF;                   \
  P7OUT = 0;                      \
  P7DIR = 0xFF;                   \
  P8OUT = 0;                      \
  P8DIR = 0xFF;

/**
 * Set GPIO pin as input.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_dir_in(port, pin) __gpio_dir_in(port, pin)

/**
 * Set GPIO pin as output.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_dir_out(port, pin) __gpio_dir_out(port, pin)

/**
 * Select GPIO pin's function.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_sel_0(port, pin) __gpio_sel_0(port, pin)
#define msp_gpio_sel_1(port, pin) __gpio_sel_1(port, pin)

/**
 * Set GPIO pin's output state.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_set(port, pin) __gpio_set(port, pin)

/**
 * Clear GPIO pin's output state.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_clear(port, pin) __gpio_clear(port, pin)

/**
 * Toggle GPIO pin's output state.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_toggle(port, pin) __gpio_toggle(port, pin)

/**
 * Produce a spike on a GPIO pin, by setting and then clearing
 * the pin's output state.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_spike(port, pin) \
  __gpio_set(port, pin);          \
  __gpio_clear(port, pin)

/**
 * Read input GPIO pin's state.
 *
 * @param port port number
 * @param pin  pin number
 */
#define msp_gpio_read(port, pin) __gpio_read(port, pin)

#endif /* INCLUDE_MSPBASE_GPIO_H */
