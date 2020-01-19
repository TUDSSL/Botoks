#include <cc1101.h>
#include <mspbase.h>
#include <mspspi.h>
#include <tpsync.h>

void cc1101_init(void (*regs_init)()) {
  // Initialize SPI:
  // - no STE (manually use GPIO for CSn)
  // - shifted phase
  // - default polarity
  // - MSB first
  // - SMCLK as source clock
  // - no clock divider
  msp_spi_init(0, 1, 0, 1, 1, 0);

  // Pull up CSn (since it's active low).
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Activate GDO0 and GDO2.
  cc1101_set_GDO();

  // Apply desired configuration.
  regs_init();
}

void cc1101_set_GDO() {
  msp_gpio_dir_in(3, 0);
  msp_gpio_dir_in(3, 2);

  // Enable pull-down resistors.
  P3REN |= BIT0 | BIT2;
  P3OUT &= ~(BIT0 | BIT2);
}

uint8_t cc1101_spi_write(uint8_t byte) {
  // Write byte via SPI.
  msp_spi_write(byte);

  // Wait for status reception.
  while (!(UCB1IFG & UCRXIFG))
    ;

  // Return status byte.
  return (uint8_t)msp_spi_read();
}

uint8_t cc1101_write_reg(uint8_t addr, uint8_t value) {
  uint8_t spi_status;

  // Pull down CSn.
  msp_gpio_clear(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Wait for slave to be ready.
  while (msp_spi_is_slave_busy())
    ;

  // Write address and register.
  spi_status = cc1101_spi_write(addr);
  spi_status = cc1101_spi_write(value);

  // Pull up CSn.
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);

  return spi_status;
}

uint8_t cc1101_write_burst_reg(uint8_t addr, uint8_t* buffer, uint8_t len) {
  uint8_t spi_status, i;

  // Pull down CSn.
  msp_gpio_clear(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Wait for slave to be ready.
  while (msp_spi_is_slave_busy())
    ;

  // Write address.
  spi_status = cc1101_spi_write(addr | CC1101_WRITE_BURST);

  // Write registers.
  for (i = 0; i < len; i++) {
    spi_status = cc1101_spi_write(buffer[i]);
  }

  // Pull up CSn.
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);

  return spi_status;
}

uint8_t cc1101_read_config_reg(uint8_t addr) {
  uint8_t header, value;

  // Pull down CSn.
  msp_gpio_clear(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Wait for slave to be ready.
  while (msp_spi_is_slave_busy())
    ;

  // Write address.
  header = addr | CC1101_READ_SINGLE;
  cc1101_spi_write(header);

  // Read register (write 0 just to activate SPI clock).
  value = cc1101_spi_write(0);

  // Pull up CSn.
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);

  return value;
}

uint8_t cc1101_read_status_reg(uint8_t addr) {
  uint8_t header, value;

  // Pull down CSn.
  msp_gpio_clear(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Wait for slave to be ready.
  while (msp_spi_is_slave_busy())
    ;

  // Write address.
  header = addr | CC1101_READ_BURST;
  cc1101_spi_write(header);

  // Read register (write 0 just to activate SPI clock).
  value = cc1101_spi_write(0);

  // Pull up CSn.
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);

  return value;
}

void cc1101_read_burst_reg(uint8_t addr, uint8_t* buffer, uint8_t len) {
  uint8_t i;

  // Pull down CSn.
  msp_gpio_clear(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Wait for slave to be ready.
  while (msp_spi_is_slave_busy())
    ;

  // Write address.
  cc1101_spi_write(addr | CC1101_READ_BURST);

  // Read registers.
  for (i = 0; i < len; i++) {
    buffer[i] = cc1101_spi_write(0);
  }

  // Pull up CSn.
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);
}

cc1101_marc_state_t cc1101_get_marc_state() {
  // Get 5 LSB of the MARCSTATE register.
  return cc1101_read_status_reg(CC1101_MARCSTATE) & 0x1F;
}

void cc1101_set_tx_power(uint8_t* PA_table) {
  // Set PA table registers.
  cc1101_write_burst_reg(CC1101_PATABLE, PA_table, 8);
}

uint8_t cc1101_send_strobe_cmd(uint8_t strobe) {
  uint8_t spi_status;

  // Pull down CSn.
  msp_gpio_clear(CC1101_CSn_PORT, CC1101_CSn_PIN);

  // Wait for slave to be ready.
  while (msp_spi_is_slave_busy())
    ;

  // Write strobe command.
  spi_status = cc1101_spi_write(strobe);

  // Pull up CSn.
  msp_gpio_set(CC1101_CSn_PORT, CC1101_CSn_PIN);

  return spi_status;
}

void cc1101_turn_on_rx() {
  // Send RX command.
  cc1101_send_strobe_cmd(CC1101_SRX);
}

void cc1101_send_packet(uint8_t* tx_buffer, uint8_t len) {
  // Fill TX FIFO with packet.
  cc1101_write_burst_reg(CC1101_TXFIFO, tx_buffer, len);

  // Send TX command.
  cc1101_send_strobe_cmd(CC1101_STX);

  // Wait for TX start.
  while (!cc1101_is_GDO0_high())
    ;

  // Wait for TX end.
  while (cc1101_is_GDO0_high())
    ;

  // Send flush TX FIFO command.
  cc1101_send_strobe_cmd(CC1101_SFTX);
}

uint8_t cc1101_receive_packet(uint8_t* rx_buffer, uint8_t len) {
  // Send flush RX FIFO command.
  cc1101_send_strobe_cmd(CC1101_SFRX);

  // Send RX command.
  cc1101_send_strobe_cmd(CC1101_SRX);

#if TPSYNC_ENABLE
  // Start tracking sync error.
  track_sync_error_start();
#if PROFILE_RX_ERROR
  msp_gpio_set(DBG_PORT, DEBUG_B);
#endif
#endif

  // Wait for RX start.
  while (!cc1101_is_GDO0_high())
    ;

#if TPSYNC_ENABLE
#if PROFILE_RX_ERROR
  msp_gpio_clear(DBG_PORT, DEBUG_B);
#endif
  // Stop tracking sync error.
  track_sync_error_stop();
#endif

  // Wait for RX end.
  while (cc1101_is_GDO0_high())
    ;

  // Read packet from RX FIFO (if FIFO not empty).
  if (cc1101_read_status_reg(CC1101_RXBYTES) & 0x7F) {
    cc1101_read_burst_reg(CC1101_RXFIFO, rx_buffer, len);
    return 1;
  }

  // Unsuccessful return if RX FIFO empty.
  return 0;
}
