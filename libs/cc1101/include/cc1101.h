#ifndef INCLUDE_CC1101_H
#define INCLUDE_CC1101_H

#include <stdint.h>

// Command strobes
#define CC1101_SRES (0x30)  // Reset CC1101 chip.
#define CC1101_SFSTXON \
  (0x31)  // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
          // If in RX (with CCA): Go to a wait state where only the synthesizer
          // is running (for quick RX / TX turnaround).
#define CC1101_SXOFF (0x32)  // Turn off crystal oscillator.
#define CC1101_SCAL \
  (0x33)  // Calibrate frequency synthesizer and turn it off. SCAL can be
          // strobed from IDLE mode without setting manual calibration mode
          // (MCSM0.FS_AUTOCAL=0).
#define CC1101_SRX \
  (0x34)  // Enable RX. Perform calibration first if coming from IDLE and
          // MCSM0.FS_AUTOCAL=1.
#define CC1101_STX \
  (0x35)  // In IDLE state: Enable TX. Perform calibration first if
          // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled: Only go to
          // TX if channel is clear.
#define CC1101_SIDLE \
  (0x36)  // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio
          // mode if applicable.
#define CC1101_SWOR \
  (0x38)  // Start automatic RX polling sequence (Wake-on-Radio) as described in
          // Section 19.5 if WORCTRL.RC_PD=0.
#define CC1101_SPWD (0x39)  // Enter power down mode when CSn goes high.
#define CC1101_SFRX \
  (0x3A)  // Flush the RX FIFO buffer. Only issue SFRX in IDLE or
          // RXFIFO_OVERFLOW states.
#define CC1101_SFTX \
  (0x3B)  // Flush the TX FIFO buffer. Only issue SFTX in IDLE or
          // TXFIFO_UNDERFLOW states.
#define CC1101_SWORRST (0x3C)  // Reset real time clock to Event1 value.
#define CC1101_SNOP \
  (0x3D)  // No operation. May be used to get access to the chip status byte.

// CC1101 configuration registers
#define CC1101_IOCFG2 (0x00)    // GDO2 Output Pin Configuration
#define CC1101_IOCFG1 (0x01)    // GDO1 Output Pin Configuration
#define CC1101_IOCFG0 (0x02)    // GDO0 Output Pin Configuration
#define CC1101_FIFOTHR (0x03)   // RX FIFO and TX FIFO Thresholds
#define CC1101_SYNC1 (0x04)     // Sync Word, High Byte
#define CC1101_SYNC0 (0x05)     // Sync Word, Low Byte
#define CC1101_PKTLEN (0x06)    // Packet Length
#define CC1101_PKTCTRL1 (0x07)  // Packet Automation Control
#define CC1101_PKTCTRL0 (0x08)  // Packet Automation Control
#define CC1101_ADDR (0x09)      // Device Address
#define CC1101_CHANNR (0x0A)    // Channel Number
#define CC1101_FSCTRL1 (0x0B)   // Frequency Synthesizer Control
#define CC1101_FSCTRL0 (0x0C)   // Frequency Synthesizer Control
#define CC1101_FREQ2 (0x0D)     // Frequency Control Word, High Byte
#define CC1101_FREQ1 (0x0E)     // Frequency Control Word, Middle Byte
#define CC1101_FREQ0 (0x0F)     // Frequency Control Word, Low Byte
#define CC1101_MDMCFG4 (0x10)   // Modem Configuration
#define CC1101_MDMCFG3 (0x11)   // Modem Configuration
#define CC1101_MDMCFG2 (0x12)   // Modem Configuration
#define CC1101_MDMCFG1 (0x13)   // Modem Configuration
#define CC1101_MDMCFG0 (0x14)   // Modem Configuration
#define CC1101_DEVIATN (0x15)   // Modem Deviation Setting
#define CC1101_MCSM2 (0x16)   // Main Radio Control State Machine Configuration
#define CC1101_MCSM1 (0x17)   // Main Radio Control State Machine Configuration
#define CC1101_MCSM0 (0x18)   // Main Radio Control State Machine Configuration
#define CC1101_FOCCFG (0x19)  // Frequency Offset Compensation Configuration
#define CC1101_BSCFG (0x1A)   // Bit Synchronization Configuration
#define CC1101_AGCCTRL2 (0x1B)  // AGC Control
#define CC1101_AGCCTRL1 (0x1C)  // AGC Control
#define CC1101_AGCCTRL0 (0x1D)  // AGC Control
#define CC1101_WOREVT1 (0x1E)   // High Byte Event0 Timeout
#define CC1101_WOREVT0 (0x1F)   // Low Byte Event0 Timeout
#define CC1101_WORCTRL (0x20)   // Wake On Radio Control
#define CC1101_FREND1 (0x21)    // Front End RX Configuration
#define CC1101_FREND0 (0x22)    // Front End TX Configuration
#define CC1101_FSCAL3 (0x23)    // Frequency Synthesizer Calibration
#define CC1101_FSCAL2 (0x24)    // Frequency Synthesizer Calibration
#define CC1101_FSCAL1 (0x25)    // Frequency Synthesizer Calibration
#define CC1101_FSCAL0 (0x26)    // Frequency Synthesizer Calibration
#define CC1101_RCCTRL1 (0x27)   // RC Oscillator Configuration
#define CC1101_RCCTRL0 (0x28)   // RC Oscillator Configuration
#define CC1101_FSTEST (0x29)    // Frequency Synthesizer Calibration Control
#define CC1101_PTEST (0x2A)     // Production Test
#define CC1101_AGCTEST (0x2B)   // AGC Test
#define CC1101_TEST2 (0x2C)     // Various Test Settings
#define CC1101_TEST1 (0x2D)     // Various Test Settings
#define CC1101_TEST0 (0x2E)     // Various Test Settings

// Status registers
#define CC1101_PARTNUM (0x30)     // Chip ID
#define CC1101_VERSION (0x31)     // Chip ID
#define CC1101_FREQEST (0x32)     // Frequency Offset Estimate from Demodulator
#define CC1101_LQI (0x33)         // Demodulator Estimate for Link Quality
#define CC1101_RSSI (0x34)        // Received Signal Strength Indication
#define CC1101_MARCSTATE (0x35)   // Main Radio Control State Machine State
#define CC1101_WORTIME1 (0x36)    // High Byte of WOR Time
#define CC1101_WORTIME0 (0x37)    // Low Byte of WOR Time
#define CC1101_PKTSTATUS (0x38)   // Current GDOx Status and Packet Status
#define CC1101_VCO_VC_DAC (0x39)  // Current Setting from PLL Calibration Module
#define CC1101_TXBYTES (0x3A)     // Underflow and Number of Bytes
#define CC1101_RXBYTES (0x3B)     // Overflow and Number of Bytes
#define CC1101_RCCTRL1_STATUS (0x3C)  // Last RC Oscillator Calibration Result
#define CC1101_RCCTRL0_STATUS (0x3D)  // Last RC Oscillator Calibration Result

// PATABLE & FIFO
#define CC1101_PATABLE (0x3E)  // PATABLE address
#define CC1101_TXFIFO (0x3F)   // TX FIFO address
#define CC1101_RXFIFO (0x3F)   // RX FIFO address

// Header byte for SPI communication
#define CC1101_WRITE_SINGLE (0x00)
#define CC1101_WRITE_BURST (0x40)
#define CC1101_READ_SINGLE (0x80)
#define CC1101_READ_BURST (0xC0)

// Various bitmasks
#define CC1101_CRC_OK (0x80)

// TX power
#define CC1101_TX_PWR_0 (0x12)  // -30 dBm, 11.9 mA
#define CC1101_TX_PWR_1 (0x0E)  // -20 dBm, 12.4 mA
#define CC1101_TX_PWR_2 (0x1D)  // -15 dBm, 13.1 mA
#define CC1101_TX_PWR_3 (0x34)  // -10 dBm, 14.4 mA
#define CC1101_TX_PWR_4 (0x60)  //   0 dBm, 15.9 mA
#define CC1101_TX_PWR_5 (0x84)  //   5 dBm, 19.4 mA
#define CC1101_TX_PWR_6 (0xC8)  //   7 dBm, 24.2 mA
#define CC1101_TX_PWR_7 (0xC0)  //  10 dBm, 29.1 mA

// Data rate (Mantissa)
#define CC1101_DATA_RATE_M_0 (0x43)  //   0.969 kBaud
#define CC1101_DATA_RATE_M_1 (0x83)  //   1.161 kBaud
#define CC1101_DATA_RATE_M_2 (0x83)  //   2.322 kBaud
#define CC1101_DATA_RATE_M_3 (0x83)  //   4.644 kBaud
#define CC1101_DATA_RATE_M_4 (0x83)  //   9.288 kBaud
#define CC1101_DATA_RATE_M_5 (0x8B)  //  19.355 kBaud
#define CC1101_DATA_RATE_M_6 (0x83)  //  38.313 kBaud
#define CC1101_DATA_RATE_M_7 \
  (0x83)  //  76.626 kBaud (this setting works well, and any data rate below it)
#define CC1101_DATA_RATE_M_8 (0x83)   // 153.252 kBaud
#define CC1101_DATA_RATE_M_9 (0x3B)   // 249.795 kBaud
#define CC1101_DATA_RATE_M_10 (0x3B)  // 499.590 kBaud

// Data rate (Exponent)
#define CC1101_DATA_RATE_E_0 (0x85)   //   0.969 kBaud
#define CC1101_DATA_RATE_E_1 (0x85)   //   1.161 kBaud
#define CC1101_DATA_RATE_E_2 (0x86)   //   2.322 kBaud
#define CC1101_DATA_RATE_E_3 (0x87)   //   4.644 kBaud
#define CC1101_DATA_RATE_E_4 (0x88)   //   9.288 kBaud
#define CC1101_DATA_RATE_E_5 (0x89)   //  19.355 kBaud
#define CC1101_DATA_RATE_E_6 (0x8A)   //  38.313 kBaud
#define CC1101_DATA_RATE_E_7 (0x8B)   //  76.626 kBaud
#define CC1101_DATA_RATE_E_8 (0x8C)   // 153.252 kBaud
#define CC1101_DATA_RATE_E_9 (0x8D)   // 249.795 kBaud
#define CC1101_DATA_RATE_E_10 (0x8E)  // 499.590 kBaud

// CSn pin
#define CC1101_CSn_PORT 5
#define CC1101_CSn_PIN 3

// Main Radio Control FSM state
typedef enum {
  SLEEP = 0x00,
  IDLE = 0x01,
  XOFF = 0x02,
  VCOON_MC = 0x03,
  REGON_MC = 0x04,
  MANCAL = 0x05,
  VCOON = 0x06,
  REGON = 0x07,
  STARTCAL = 0x08,
  BWBOOST = 0x09,
  FS_LOCK = 0x0A,
  IFADCON = 0x0B,
  ENDCAL = 0x0C,
  RX = 0x0D,
  RX_END = 0x0E,
  RX_RST = 0x0F,
  TXRX_SWITCH = 0x10,
  RXFIFO_OVERFLOW = 0x11,
  FSTXON = 0x12,
  TX = 0x13,
  TX_END = 0x14,
  RXTX_SWITCH = 0x15,
  TXFIFO_UNDERFLOW = 0x16
} cc1101_marc_state_t;

/**
 * Initialize CC1101 radio for RX/TX operation.
 *
 * @param regs_init user-defined function where CC1101's register are
 * initialized
 */
void cc1101_init(void (*regs_init)());

/**
 * Set 2 GPIO's as input pins for GDO0 and GDO2.
 *
 * P3.0 <- GDO0
 * P3.2 <- GDO2
 */
void cc1101_set_GDO(void);

/**
 * Check if GDO0 is high.
 */
#define cc1101_is_GDO0_high() msp_gpio_read(3, 0)

/**
 * Write a CC1101 register via SPI.
 *
 * @param addr  register address
 * @param value value to be written
 *
 * @return chip status byte
 */
uint8_t cc1101_write_reg(uint8_t addr, uint8_t value);

/**
 * Write a CC1101 register via SPI in burst mode.
 *
 * @param addr   starting register address
 * @param values array of values to be written
 * @param len    number of registers to write
 *
 * @return chip status byte
 */
uint8_t cc1101_write_burst_reg(uint8_t addr, uint8_t* values, uint8_t len);

/**
 * Read a CC1101 configuration register via SPI.
 *
 * @param addr register address
 *
 * @return register value
 */
uint8_t cc1101_read_config_reg(uint8_t addr);

/**
 * Read a CC1101 status register via SPI.
 *
 * @param addr register address
 *
 * @return register value
 */
uint8_t cc1101_read_status_reg(uint8_t addr);

/**
 * Read a CC1101 register via SPI in burst mode.
 *
 * @param addr   starting register address
 * @param values buffer to read the registers to
 * @param len    number of registers to read
 */
void cc1101_read_burst_reg(uint8_t addr, uint8_t* values, uint8_t len);

/**
 * Get CC1101's FSM state.
 *
 * @return FSM state
 */
cc1101_marc_state_t cc1101_get_marc_state(void);

/**
 * Set CC1101's transmission power.
 *
 * @param PA_table array containing the 8 PA table values
 */
void cc1101_set_tx_power(uint8_t* PA_table);

/**
 * Send strobe command to CC1101.
 *
 * @param strobe command to send
 *
 * @return chip status byte
 */
uint8_t cc1101_send_strobe_cmd(uint8_t strobe);

/**
 * Turn on RX mode.
 */
void cc1101_turn_on_rx(void);

/**
 * Send packet over radio.
 *
 * @param tx_buffer buffer containing the packet
 * @param len       packet length, in bytes
 */
void cc1101_send_packet(uint8_t* tx_buffer, uint8_t len);

/**
 * Receive packet over radio.
 *
 * @param rx_buffer buffer to store the packet to
 * @param len       packet length, in bytes
 *
 * @return 1 if successful, 0 otherwise (i.e. RX FIFO empty)
 */
uint8_t cc1101_receive_packet(uint8_t* rx_buffer, uint8_t len);

#endif /* INCLUDE_CC1101_H */
