#include <mspbase.h>
#include <mspprintf.h>
#include <platform.h>
#include <stdbool.h>
#include <stdint.h>
#include <zl70550_regs.h>

extern uint8_t longTransfer(bool write, uint16_t address, uint8_t data);
extern void zl70550_read_irq(uint8_t* buffer);
extern void zl70550_set_frequency();
extern uint8_t zl70550_spi_write(uint8_t byte);

#define calibRegionSize 24
__nv uint8_t FRAM_calibration_values[calibRegionSize + 1];

uint8_t zl70550_current_reference_trimming();
uint8_t zl70550_rco_tuning();
uint8_t zl70550_vco_amplitude_trim();
uint8_t zl70550_vco_frequency_trim();
uint8_t zl70550_block_peak_trim();
uint8_t zl70550_antenna_tuning();
uint8_t zl70550_LNA_load_tuning();
uint8_t zl70550_RX_Filter_trimming();
uint8_t zl70550_FSK_deviation_trimming();

// requires a 18 (calibRegionSize) byte buffer.
void zl70550_read_calibration(uint8_t* buffer) {
  uint8_t cmd_h = ((VCO_FRQ_RX_TRIM_L >> 8) & 0x0F) | 0b00100000;
  uint8_t cmd_l = VCO_FRQ_RX_TRIM_L & 0xFF;

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  zl70550_spi_write(cmd_h);
  zl70550_spi_write(cmd_l);
  zl70550_spi_write(0);

  for (int i = 0; i < calibRegionSize; i++) {
    buffer[i] = zl70550_spi_write(0);
  }

  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  msp_printf("ZL70550: Current calib:\n");
  for (int i = 0; i < calibRegionSize; i++) {
    msp_printf("%x ", buffer[i]);
  }
  msp_printf("\n");
}

void zl70550_backup_calibration() {
  uint8_t calibBuffer[calibRegionSize];
  zl70550_read_calibration(calibBuffer);
  for (int i = 0; i < calibRegionSize; i++) {
    FRAM_calibration_values[i] = calibBuffer[i];
  }
  FRAM_calibration_values[calibRegionSize] = 0xFE;
}

bool zl70550_has_valid_calibration() {
  return FRAM_calibration_values[calibRegionSize] == 0xFE;
}

void zl70550_restore_calibration() {
  // starting calib restore

  // msp_printf("ZL70550: Current calib in mem:\n");
  // for (int i = 0; i < calibRegionSize; i++) {
  //  msp_printf("%x ", FRAM_calibration_values[i]);
  //}
  // msp_printf("\n");

  // uint8_t calibBuffer[calibRegionSize];
  // zl70550_read_calibration(calibBuffer);

  longTransfer(true, VCO_FRQ_RX_TRIM_L, FRAM_calibration_values[0]);
  longTransfer(true, VCO_FRQ_RX_TRIM_H, FRAM_calibration_values[1]);
  longTransfer(true, VCO_FRQ_TXPAOFF_TRIM_L, FRAM_calibration_values[2]);
  longTransfer(true, VCO_FRQ_TXPAOFF_TRIM_H, FRAM_calibration_values[3]);
  longTransfer(true, VCO_FRQ_TXPAON_TRIM_L, FRAM_calibration_values[4]);
  longTransfer(true, VCO_FRQ_TXPAON_TRIM_H, FRAM_calibration_values[5]);

  longTransfer(true, VCO_FRQ_BAND_TRIM, FRAM_calibration_values[8]);
  longTransfer(true, MOD_DAC_TRIM, FRAM_calibration_values[9]);
  longTransfer(true, RCOSC_FREQ_TRIM, FRAM_calibration_values[12]);
  longTransfer(true, LNA_FRQ_TRIM, FRAM_calibration_values[13]);
  longTransfer(true, LNA_BIAS_TRIM, FRAM_calibration_values[14]);
  longTransfer(true, IREF_TRIM, FRAM_calibration_values[15]);
  longTransfer(true, XO_TRIM, FRAM_calibration_values[16]);
  longTransfer(true, GAUS_TRIM, FRAM_calibration_values[19]);
  longTransfer(true, VCO_AMP_TRIM, FRAM_calibration_values[20]);
  longTransfer(true, ANT_TRIM, FRAM_calibration_values[21]);
  longTransfer(true, RX_PKDET_TRIM, FRAM_calibration_values[22]);
  longTransfer(true, DC_CNTR_TRIM, FRAM_calibration_values[23]);

  // confirm
  // zl70550_read_calibration(calibBuffer);
}

void zl70550_calibrate() {
  if (zl70550_has_valid_calibration()) {
    zl70550_restore_calibration();
    //    msp_printf("ZL70550: Restoring calibration....\n");
  } else {
    // enable trim done and fail interrupt
    msp_printf(
        "ZL70550: Did not find calibration data. Starting calibration... \n");

    longTransfer(true, IRQ_EN1, 0b01100000);
    uint8_t buffer[3];
    zl70550_read_irq(buffer);
    msp_printf("ZL70550 IRQ0: %x IRQ1: %x IRQ2: %x \n", buffer[0], buffer[1],
               buffer[2]);

    uint8_t status = 0;
    status |= zl70550_current_reference_trimming();
    status |= zl70550_rco_tuning();
    status |= zl70550_vco_amplitude_trim();
    status |= zl70550_vco_frequency_trim();
    status |= zl70550_block_peak_trim();

    status |= zl70550_antenna_tuning();
    status |= zl70550_LNA_load_tuning();
    status |= zl70550_RX_Filter_trimming();
    status |= zl70550_FSK_deviation_trimming();

    if (!status) {
      zl70550_backup_calibration();
      msp_printf("ZL70550: Calibration completed successfully\n\n");
    } else {
      // one of the calibration steps failed.
      msp_printf("ZL70550: Calibration failed\n\n");
    }
  }
}

uint8_t zl70550_vco_amplitude_trim() {
  // VCO Amplitude Trim
  uint8_t pretune = longTransfer(false, VCO_AMP_TRIM, 0);
  zl70550_set_frequency();

  longTransfer(true, MAC_CTRL, 0x00);

  longTransfer(true, TRIM_CMD, 0x06);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, VCO_AMP_TRIM, 0);

  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("VCO Amplitude Trim success: %x, pre %x, post %x \n", buffer[1],
               pretune, posttune);
    return 0;
  } else {
    msp_printf("VCO Amplitude Trim failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_block_peak_trim() {
  // Blocker peak detector offset trimming
  uint8_t pretune = longTransfer(false, RX_PKDET_TRIM, 0);

  longTransfer(true, TRIM_CMD, 0x08);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, RX_PKDET_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf(
        "Blocker peak detector offset trimming success: %x, pre %x, post %x \n",
        buffer[1], pretune, posttune);
    return 0;
  } else {
    msp_printf("Blocker peak detector offset trimming failed! %x \n",
               buffer[1]);
    return 1;
  }
}

// buffer of uart8_t size 7.
void zl70550_vco_calib_transfer(bool write, uint8_t* buffer) {
  uint8_t cmd_h =
      ((uint8_t)write << 7) | ((VCO_FRQ_RX_TRIM_L >> 8) & 0x0F) | 0b00100000;
  uint8_t cmd_l = VCO_FRQ_RX_TRIM_L & 0xFF;

  // Pull down CSn.
  msp_gpio_clear(zl70550_CSn_PORT, zl70550_CSn_PIN);

  zl70550_spi_write(cmd_h);
  zl70550_spi_write(cmd_l);
  if (write) {
    for (int i = 0; i < 6; i++) {
      zl70550_spi_write(buffer[i]);
    }
  } else {
    zl70550_spi_write(0);
    for (int i = 0; i < 6; i++) {
      buffer[i] = zl70550_spi_write(0);
    }
  }
  // Pull up CSn.
  msp_gpio_set(zl70550_CSn_PORT, zl70550_CSn_PIN);

  if (write) {
    longTransfer(write, VCO_FRQ_BAND_TRIM, buffer[6]);
  } else {
    buffer[6] = longTransfer(write, VCO_FRQ_BAND_TRIM, 0);
  }
}

uint8_t zl70550_vco_frequency_trim() {
  uint8_t pretune[7];
  zl70550_vco_calib_transfer(false, pretune);

  longTransfer(true, TRIM_CMD, 0x02);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune[7];
  zl70550_vco_calib_transfer(false, posttune);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("VCO Frequency Trim success: %x \n", buffer[1]);
    msp_printf("Pre %x, %x : %x, %x : %x, %x : %x\n", pretune[0], pretune[1],
               pretune[2], pretune[3], pretune[4], pretune[5], pretune[6]);
    msp_printf("Post %x, %x : %x, %x : %x, %x : %x\n", posttune[0], posttune[1],
               posttune[2], posttune[3], posttune[4], posttune[5], posttune[6]);
    return 0;
  } else {
    msp_printf("VCO Frequency Trim failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_rco_tuning() {
  // RCO tuning
  uint8_t pretune = longTransfer(false, RCOSC_FREQ_TRIM, 0);

  longTransfer(true, TRIM_CMD, 0x0C);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, RCOSC_FREQ_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("RCO Tuning success: %x, pre %x, post %x \n", buffer[1], pretune,
               posttune);
    return 0;
  } else {
    msp_printf("RCO Tuning failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_current_reference_trimming() {
  // trim current reference
  uint8_t pretune = longTransfer(false, IREF_TRIM, 0);

  longTransfer(true, TRIM_CMD, 0x01);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, IREF_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("Calibrated current reference success: %x, pre %x, post %x  \n",
               buffer[1], pretune, posttune);
    return 0;
  } else {
    msp_printf("Calibrated current reference failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_antenna_tuning() {
  // start antenna tuning
  uint8_t pretune = longTransfer(false, ANT_TRIM, 0);

  longTransfer(true, LNA_GAIN, 0x00);
  longTransfer(true, LNA_BIAS_TRIM, 0x01);
  longTransfer(true, PA_PWR_LEVEL, 0x80);

  longTransfer(true, TRIM_CMD, 0x0A);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, ANT_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("Antenna tuning success: %x, pre %x, post %x  \n", buffer[1],
               pretune, posttune);
    return 0;
  } else {
    msp_printf("Antenna tuning failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_LNA_load_tuning() {
  uint8_t pretune = longTransfer(false, LNA_FRQ_TRIM, 0);

  longTransfer(true, TRIM_CMD, 0x09);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, LNA_FRQ_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("LNA load tuning success: %x, pre %x, post %x  \n", buffer[1],
               pretune, posttune);
    return 0;
  } else {
    msp_printf("LNA load tuning failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_RX_Filter_trimming() {
  uint8_t pretune = longTransfer(false, DC_CNTR_TRIM, 0);
  uint8_t pretune2 = longTransfer(false, GAUS_TRIM, 0);

  longTransfer(true, TRIM_CMD, 0x0B);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, DC_CNTR_TRIM, 0);
  uint8_t posttune2 = longTransfer(false, GAUS_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("RX Filter trimming success: %x, pre %x : %x, post %x : %x  \n",
               buffer[1], pretune, pretune2, posttune, posttune2);
    return 0;
  } else {
    msp_printf("RX Filter trimming failed! %x \n", buffer[1]);
    return 1;
  }
}

uint8_t zl70550_FSK_deviation_trimming() {
  uint8_t pretune = longTransfer(false, MOD_DAC_TRIM, 0);

  longTransfer(true, MAN_GLOBAL_EN, 0x04);
  __delay_cycles(2000);  // delay 2 ms

  longTransfer(true, TRIM_CMD, 0x07);
  while (!(msp_gpio_read(zl70550_IRQ_PORT, zl70550_IRQ_PIN))) {
    __delay_cycles(100);
  }

  uint8_t buffer[3];
  zl70550_read_irq(buffer);

  uint8_t posttune = longTransfer(false, MOD_DAC_TRIM, 0);
  if ((buffer[1] & 0b01000000) && !(buffer[1] & 0b00100000)) {
    msp_printf("FSK deviation trimming success: %x, pre %x, post %x  \n",
               buffer[1], pretune, posttune);
    return 0;
  } else {
    msp_printf("FSK deviation trimming failed! %x \n", buffer[1]);
    return 1;
  }

  longTransfer(true, MAN_GLOBAL_EN, 0x00);
}
