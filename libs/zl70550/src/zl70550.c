#include <mspbase.h>
#include <mspprintf.h>
#include <mspspi.h>
#include <platform.h>
#include <stdbool.h>
#include <string.h>
#include <zl70550.h>
#include <zl70550_regs.h>

#define EncodeBufferSize 100
uint8_t encodeBuffer[EncodeBufferSize];
uint8_t status, statusRL1, statusRL2;

void zl70550_read_irq(uint8_t* buffer);

uint8_t zl70550_spi_write(uint8_t byte) {
  // Write byte via SPI.
  msp_spi_write(byte);

  // Wait for status reception.
  while (!msp_spi_data_availible())
    ;

  // Return status byte.
  return (uint8_t)msp_spi_read();
}

// do a short transfer, write 0 is regarded as read operation.
// data may be zero for read mode.
uint8_t shortTransfer(bool write, uint8_t address, uint8_t data) {
  uint8_t result;

  encodeBuffer[0] = ((uint8_t)write << 7) | (address & 0b00111111) | 0b01000000;
  if (write) {
    encodeBuffer[1] = data;
  } else {
    encodeBuffer[1] = 0;
  }
  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // Wait for slave to be ready.
  // while (msp_spi_is_slave_busy());

  status = zl70550_spi_write(encodeBuffer[0]);
  result = zl70550_spi_write(encodeBuffer[1]);

  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  return result;
}

// do a short transfer, write 0 is regarded as read operation.
// data may be zero for read mode.
uint8_t longTransfer(bool write, uint16_t address, uint8_t data) {
  uint8_t result = 0;
  encodeBuffer[0] =
      ((uint8_t)write << 7) | ((address >> 8) & 0x0F) | 0b00100000;
  encodeBuffer[1] = address & 0xFF;
  if (write) {
    encodeBuffer[2] = data;
  } else {
    encodeBuffer[2] = 0;
    encodeBuffer[3] = 0;  // one dead byte
  }

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // Wait for slave to be ready.
  // while (msp_spi_is_slave_busy());

  status = zl70550_spi_write(encodeBuffer[0]);

  if (write) {
    zl70550_spi_write(encodeBuffer[1]);
    zl70550_spi_write(encodeBuffer[2]);
  } else {
    statusRL1 = zl70550_spi_write(encodeBuffer[1]);
    statusRL2 = zl70550_spi_write(encodeBuffer[2]);
    result = zl70550_spi_write(encodeBuffer[3]);
  }
  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  return result;
}

uint8_t packetTransferEncode(bool write, uint8_t* data, uint8_t length) {
  encodeBuffer[0] = ((uint8_t)write << 7) | 0b1000;
  if (write) {
    memcpy(&encodeBuffer[1], data, length);
  } else {
    memset(&encodeBuffer[1], 0, length + 1);  // one dead byte processing time
  }
  return write ? length + 1 : length + 2;
}

void sendCommand(Commands command) {
  encodeBuffer[0] = 0x80 | command;

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  status = zl70550_spi_write(encodeBuffer[0]);

  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);
}

void zl70550_init() {
  // Initialize SPI:
  // - no STE (manually use GPIO for CSn)
  // - shifted phase
  // - default polarity
  // - MSB first
  // - SMCLK as source clock
  // - no clock divider
  msp_spi_init(0, 1, 0, 1, 1, 0);

  // turn power on
  msp_gpio_set(zl70550_POWER_PORT, zl70550_POWER_PIN);

  // Pull up CSn (since it's active low).
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);
  msp_gpio_set(zl70550_RESETn_PORT, zl70550_RESETn_PIN);

  msp_gpio_dir_in(zl70550_IRQ_PORT, zl70550_IRQ_PIN);

  zl70550_reset();
}

void zl70550_reset() {
  msp_gpio_clear(zl70550_RESETn_PORT, zl70550_RESETn_PIN);
  __delay_cycles(20);
  msp_gpio_set(zl70550_RESETn_PORT, zl70550_RESETn_PIN);

  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);
  __delay_cycles(5);
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // wait for IRQ to become active indicating the device has powered on.
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }
  // now we can communicate to the device
}

void zl70550_restore_state() {
  longTransfer(true, FSK_DEV_TRIM_TARGET, 0x76);  // europe
  longTransfer(true, SYNC_THRESH, 0x15);

  // 10 pre-amble bytes.
  longTransfer(true, PREAM_LEN, 0x0A);
  longTransfer(true, ADJ_FAST, 0x25);
  longTransfer(true, ADJ_SLOW, 0x25);
  longTransfer(true, PREAM_LOCK_CNT, 0x07);
  longTransfer(true, TX_PLL_DLY_CNT, 0x0F);
  longTransfer(true, RX_PLL_DLY_CNT, 0x16);
  longTransfer(true, CW_DLY_CNT, 0x05);
  longTransfer(true, SLO_DLY_CNT, 0xFF);
  longTransfer(true, PA_OFF_DLY_CNT, 0x00);
  longTransfer(true, ACK_TIME_LIMIT, 0x0C);
  longTransfer(true, PKT_TIME_LIMIT, 0x0C);
  longTransfer(true, SYS_CLK_DIV, 0x14);
  longTransfer(true, PLL_CLK_DIV_CNT, 0x50);

  // require pre-amble
  longTransfer(true, PHY_RX_MODE_SEL, 0b00000101);

  longTransfer(true, SYNC_DLY_CNT, 0x0F);
  longTransfer(true, PREAM_DET_CTRL, 0x33);

  // 3 sync bytes
  longTransfer(true, DPORT_CTRL, 0x1B);
  longTransfer(true, ADC_RSSI_THRESH, 0x01);
  longTransfer(true, LNA_GAIN, 0x0F);
  longTransfer(true, ANA_CTRL0, 0x84);
  longTransfer(true, VCO_BUF_BIAS, 0x03);
  longTransfer(true, VCO_CMP_VREF_CTRL, 0x01);
}

void zl70550_set_frequency() {
  // 869.7 MHz always write M first!
  longTransfer(true, SYNTH_M_DIV, 0xA1);
  longTransfer(true, SYNTH_A_DIV, 0x13);

  longTransfer(true, VCO_FRQ_BAND_TRIM, 4);
}

void zl70550_raw_transmit() {
  // transmit raw mode no hamming encoding no retries.
  longTransfer(true, TX_CTRL0, 0b00000001);
  longTransfer(true, TX_CTRL1, 0b00001000);
  longTransfer(true, TX_CTRL2, 0b00010000);
  // disable csma
  longTransfer(true, CSMA_CTRL, 0b00000000);
  // transmit only when full packet is in the buffer
  longTransfer(true, SPI_CTRL, 0b00000110);
  // enable tx packet done irq
  // longTransfer(true, IRQ_EN2, 0b11000000);

  longTransfer(true, IRQ_EN0, 0x0f);
  longTransfer(true, IRQ_EN1, 0xff);
  longTransfer(true, IRQ_EN2, 0xff);

  // enable mac
  longTransfer(true, MAC_CTRL, 0x02);

  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];

  zl70550_read_irq(buffer);
  msp_printf("ZL70550 Pre transmit IRQ0: %x IRQ1: %x IRQ2: %x \n", buffer[0],
             buffer[1], buffer[2]);

  uint8_t test = longTransfer(false, MAC_CTRL, 0);
  msp_printf("ZL70550 Pre transmit MAC_CTRL: %x \n", test);

  test = longTransfer(false, SPIR_LOCAL_STAT, 0);
  msp_printf("ZL70550 Pre transmit SPIR_LOCAL_STAT: %x \n", test);

  test = longTransfer(false, SPI_CTRL, 0);
  msp_printf("ZL70550 Pre transmit  SPI_CTRL: %x \n", test);

  // encode the transfer
  uint8_t txBuffer[] = {0x01, 0x02};
  uint8_t packetLength;
  packetLength = packetTransferEncode(true, txBuffer, sizeof(txBuffer));

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // Wait for slave to be ready.
  // while (msp_spi_is_slave_busy());

  for (int i = 0; i < packetLength; i++) {
    zl70550_spi_write(encodeBuffer[i]);
  }

  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  zl70550_read_irq(buffer);
  msp_printf("ZL70550 Post transmit IRQ0: %x IRQ1: %x IRQ2: %x \n", buffer[0],
             buffer[1], buffer[2]);
}

void zl70550_packet_transmit(uint8_t* data, uint16_t length) {
  // transmit user packet mode, hamming encoding no retries.
  longTransfer(true, TX_CTRL0, 0b00001010);
  // transmit once
  longTransfer(true, TX_CTRL1, 0b00001000);
  // auto hdr enabled (length and preamble frame),  two byte fcs
  longTransfer(true, TX_CTRL2, 0b00100010);

  // transmit forever:
  // longTransfer(true, TX_CTRL1, 0b00000000);
  // auto hdr enabled (length and preamble frame),  two byte fcs
  // longTransfer(true, TX_CTRL2, 0b00110010);

  // disable csma
  longTransfer(true, CSMA_CTRL, 0b00000000);

  // transmit only when full packet is in the buffer
  longTransfer(true, SPI_CTRL, 0b00000110);
  // enable tx packet done irq
  // longTransfer(true, IRQ_EN2, 0b11000000);

  longTransfer(true, IRQ_EN0, 0x00);
  longTransfer(true, IRQ_EN1, 0b10000000);
  longTransfer(true, IRQ_EN2, 0b00000010);

  // enable mac auto power down.
  // longTransfer(true, MAC_CTRL, 0x12);
  // enable mac without auto power down.
  longTransfer(true, MAC_CTRL, 0x02);

  // wait until pll becomes stable
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  // clear the IRQ
  zl70550_read_irq(buffer);
  // msp_printf("ZL70550 Pre transmit, status: %x IRQ0: %x IRQ1: %x IRQ2: %x
  // \n",status, buffer[0], buffer[1], buffer[2]);

  // encode the transfer
  uint8_t packetLength;
  packetLength = packetTransferEncode(true, data, length);

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // Wait for slave to be ready.
  // while (msp_spi_is_slave_busy());

  for (int i = 0; i < packetLength; i++) {
    zl70550_spi_write(encodeBuffer[i]);
  }

  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // while(!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))){
  //   __delay_cycles(100);
  // }
  //
  // zl70550_read_irq(buffer);
  //   msp_printf("ZL70550 Post transmit, status: %x IRQ0: %x IRQ1: %x IRQ2: %x
  //   \n",status, buffer[0], buffer[1], buffer[2]);
}

// Currently does not check on buffer size
void zl70550_packet_receive(uint8_t* rx_buffer, bool forever) {
  // receive user packet mode, hamming encoding no retries.
  longTransfer(true, RX_CTRL0, 0b00001010);
  // no tx reply/ack rx only, continues receive, two byte fcs
  longTransfer(true, RX_CTRL1, 0b01001010);
  // ignore addresses, only copy actual data to Rx buffer.
  longTransfer(true, RX_CTRL2, 0b00100100);

  longTransfer(true, IRQ_EN0, 0b00000000);
  // synth interrupt enable
  longTransfer(true, IRQ_EN1, 0b10000000);
  // done/fail interrupts
  longTransfer(true, IRQ_EN2, 0b00000011);

  // enable mac
  longTransfer(true, MAC_CTRL, 0x02);

  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);
  // msp_printf("ZL70550: initial receive status: %x, %x, %x, IRQ0: %x IRQ1: %x
  // IRQ2: %x \n", status, statusRL1, statusRL2, buffer[0], buffer[1],
  // buffer[2]);

  sendCommand(StartReceiver);

  bool validPacketReceived = false;
  while (!validPacketReceived || forever) {
    while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
      __delay_cycles(100);
    }

    zl70550_read_irq(buffer);
    // msp_printf("ZL70550: receive  status: %x, %x, %x, IRQ0: %x IRQ1: %x IRQ2:
    // %x \n", status, statusRL1, statusRL2, buffer[0], buffer[1], buffer[2]);

    // todo maybe check for rxc_pkt_pass (passed crc)
    uint16_t receivedBytes = (((uint16_t)status & 0x01) << 8) | statusRL1;
    if (receivedBytes && (buffer[2] & rx_pkt_rdy_irq_mask) &&
        (buffer[2] & cmd_done_irq_mask)) {
      uint16_t packetLength;
      // msp_printf("ZL70550: received packet, %d bytes\n", receivedBytes);

      packetLength = packetTransferEncode(false, NULL, receivedBytes);

      // Pull down CSn.
      msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

      for (int i = 0; i < packetLength; i++) {
        if (i == 0) {
          status = zl70550_spi_write(encodeBuffer[i]);
        } else if (i == 1) {
          zl70550_spi_write(encodeBuffer[i]);
        } else if (i >= 2) {
          rx_buffer[i - 2] = zl70550_spi_write(encodeBuffer[i]);
        }
      }
      // Pull up CSn.
      msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

      validPacketReceived = true;

      //      uint16_t time, tier, seqnr;
      //      time =  rx_buffer[0] | ((uint16_t) rx_buffer[1] << 8);
      //      tier =  rx_buffer[2] | ((uint16_t) rx_buffer[3] << 8);
      //      seqnr =  rx_buffer[4] | ((uint16_t) rx_buffer[5] << 8);
      //       msp_printf("Time: %u, Tier: %u, SeqNR %u \n", time, tier, seqnr);
      //       for (int i = 0; i < packetLength - 2; i++) {
      //         msp_printf(" %x", rx_buffer[i]);
      //       }
      //       msp_printf("\n");

    } else {
      // msp_printf("ZL70550: CRC fail, %d bytes\n", receivedBytes);
    }
  }

  // you may go to sleep now.. :)
  shortTransfer(true, SPIR_PWRDWN_REQ, 0xDE);
}

// buffer should be 3 bytes!
void zl70550_read_irq(uint8_t* buffer) {
  encodeBuffer[0] = ((IRQ0 >> 8) & 0x0F) | 0b00100000;
  encodeBuffer[1] = IRQ0 & 0xFF;
  encodeBuffer[2] = 0;  // one dead byte
  encodeBuffer[3] = 0;
  encodeBuffer[4] = 0;
  encodeBuffer[5] = 0;

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  // Wait for slave to be ready.
  // while (msp_spi_is_slave_busy());

  status = zl70550_spi_write(encodeBuffer[0]);
  statusRL1 = zl70550_spi_write(encodeBuffer[1]);
  statusRL2 = zl70550_spi_write(encodeBuffer[2]);

  buffer[0] = zl70550_spi_write(encodeBuffer[3]);
  buffer[1] = zl70550_spi_write(encodeBuffer[4]);
  buffer[2] = zl70550_spi_write(encodeBuffer[5]);

  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);
}

uint8_t zl70550_whoAmI() { return shortTransfer(false, SPIR_CHIP_ID, 0); }
