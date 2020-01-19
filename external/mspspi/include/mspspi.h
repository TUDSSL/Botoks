#ifndef INCLUDE_MSPSPI_H
#define INCLUDE_MSPSPI_H

#include <msp430.h>
#include <platform.h>

#if HWREV > 1
/**
 * Initialize SPI (UCB1) with:
 * - master mode
 * - 8-bit data
 * - 4-pin SPI with/without STE (active low)
 *
 *  pin | dir | SEL1 | SEL0 | function
 * ------------------------------------
 * P5.0 | OUT |   0  |   1  |   SIMO
 * P5.1 |  IN |   0  |   1  |   SOMI
 * P5.2 | OUT |   0  |   1  |    CLK
 * P5.3 | OUT |   0  |  1/0 |  STE/CSn
 *
 * @param ste      enable STE
 * @param phase    CLK phase
 * @param polarity CLK polarity
 * @param dir      0: LSB first, 1: MSB first
 * @param sclk     source clock (0: ACLK, 1: SMCLK)
 * @param dclk     clock divider
 */
#define msp_spi_init(ste, phase, polarity, dir, sclk, dclk)       \
  {                                                               \
    UCB0CTLW0 |= UCSWRST;                                         \
    msp_gpio_sel_1(SPI_MOSI_PORT, SPI_MOSI_PIN);                  \
    msp_gpio_dir_out(SPI_MOSI_PORT, SPI_MOSI_PIN);                \
    msp_gpio_sel_1(SPI_MISO_PORT, SPI_MISO_PIN);                  \
    msp_gpio_dir_in(SPI_MISO_PORT, SPI_MISO_PIN);                 \
    msp_gpio_sel_1(SPI_CLK_PORT, SPI_CLK_PIN);                    \
    msp_gpio_dir_out(SPI_CLK_PORT, SPI_CLK_PIN);                  \
    if (ste) {                                                    \
      msp_gpio_sel_1(SPI_CS_PORT, SPI_CS_PIN);                    \
      msp_gpio_dir_out(SPI_CS_PORT, SPI_CS_PIN);                  \
    }                                                             \
    UCB0CTLW0 |= (phase ? UCCKPH : 0) | (polarity ? UCCKPL : 0) | \
                 (dir ? UCMSB : 0) | UCMST | UCMODE_2 | UCSYNC |  \
                 (sclk ? UCSSEL__SMCLK : UCSSEL__ACLK) | UCSTEM;  \
    UCB0BRW = dclk;                                               \
    UCB0CTLW0 &= ~UCSWRST;                                        \
  }

/**
 * Transfer a byte.
 */
#define msp_spi_write(value) \
  {                          \
    UCB0IFG &= ~UCRXIFG;     \
    UCB0TXBUF = value;       \
  }

/**
 * Read a byte.
 */
#define msp_spi_read() UCB0RXBUF

#define msp_spi_data_availible() UCB0IFG& UCRXIFG

#elif HWREV == 1

/**
 * Initialize SPI (UCB1) with:
 * - master mode
 * - 8-bit data
 * - 4-pin SPI with/without STE (active low)
 *
 *  pin | dir | SEL1 | SEL0 | function
 * ------------------------------------
 * P5.0 | OUT |   0  |   1  |   SIMO
 * P5.1 |  IN |   0  |   1  |   SOMI
 * P5.2 | OUT |   0  |   1  |    CLK
 * P5.3 | OUT |   0  |  1/0 |  STE/CSn
 *
 * @param ste      enable STE
 * @param phase    CLK phase
 * @param polarity CLK polarity
 * @param dir      0: LSB first, 1: MSB first
 * @param sclk     source clock (0: ACLK, 1: SMCLK)
 * @param dclk     clock divider
 */
#define msp_spi_init(ste, phase, polarity, dir, sclk, dclk)       \
  {                                                               \
    UCB1CTLW0 |= UCSWRST;                                         \
    msp_gpio_sel_0(SPI_MOSI_PORT, SPI_MOSI_PIN);                  \
    msp_gpio_dir_out(SPI_MOSI_PORT, SPI_MOSI_PIN);                \
    msp_gpio_sel_0(SPI_MISO_PORT, SPI_MISO_PIN);                  \
    msp_gpio_dir_in(SPI_MISO_PORT, SPI_MISO_PIN);                 \
    msp_gpio_sel_0(SPI_CLK_PORT, SPI_CLK_PIN);                    \
    msp_gpio_dir_out(SPI_CLK_PORT, SPI_CLK_PIN);                  \
    if (ste) {                                                    \
      msp_gpio_sel_0(SPI_CS_PORT, SPI_CS_PIN);                    \
      msp_gpio_dir_out(SPI_CS_PORT, SPI_CS_PIN);                  \
    }                                                             \
    UCB1CTLW0 |= (phase ? UCCKPH : 0) | (polarity ? UCCKPL : 0) | \
                 (dir ? UCMSB : 0) | UCMST | UCMODE_2 | UCSYNC |  \
                 (sclk ? UCSSEL__SMCLK : UCSSEL__ACLK) | UCSTEM;  \
    UCB1BRW = dclk;                                               \
    UCB1CTLW0 &= ~UCSWRST;                                        \
  }

/**
 * Transfer a byte.
 */
#define msp_spi_write(value) \
  {                          \
    UCB1IFG &= ~UCRXIFG;     \
    UCB1TXBUF = value;       \
  }

/**
 * Read a byte.
 */
#define msp_spi_read() UCB1RXBUF

#define msp_spi_data_availible() UCB1IFG& UCRXIFG

#endif

/**
 * Check if slave is busy (i.e. SOMI/P5.1 is high).
 */
#define msp_spi_is_slave_busy() msp_gpio_read(SPI_MISO_PORT, SPI_MISO_PIN)

#endif /* INCLUDE_MSPSPI_H */
