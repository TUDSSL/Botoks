#ifndef INCLUDE_zl70550_calib_H
#define INCLUDE_zl70550_calib_H
/**
 * ZL70550 Calibration
 */

/**
 * Calibration routine
 *
 * Execute the required calibration routine on the ZL70550.
 * It calibrates the following:
 *  - Current reference
 *  - RCO
 *  - VCO amplitude
 *  - VCO frequency
 *  - Peak detector;
 *  - Antenna output
 *  - LNA
 *  - RC filter
 *  - FSK frequency deviation
 *
 *  @ingroup ZL70550
 */
void zl70550_calibrate();

#endif /* INCLUDE_zl70550_calib_H */
