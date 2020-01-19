#ifndef INCLUDE_zl70550_H
#define INCLUDE_zl70550_H

#include <stdint.h>
#include <zl70550_calib.h>

/**
 * SPI commands to the ZL70550 signaling different control options
 */
typedef enum Commands {
  StartTransmit = 0b1001,  //!< StartTransmit Starts the transmission
  DataRequest = 0b1100,    //!< DataRequest
  StartReceiver = 0b1101,  //!< StartReceiver Enables receiving
  Abort = 0xF              //!< Abort
} Commands;

/**
 * @defgroup ZL70550 ZL70550
 */

/**
 * Initialize ZL70550
 *
 * Initializes the GPIO and SPI to communicate with the ZL70550.
 * Afterwards the ZL70550 is reset.
 *
 * @ingroup ZL70550
 */
void zl70550_init();

/**
 * Reset the ZL70550
 *
 * Resets the ZL70550 by pulling the reset line low.
 *
 */
void zl70550_reset();

/**
 * Restore the ZL70550's registers to a know state
 *
 * Initializes the common registers required for operation.
 * Operating frequency still has to be configured separately.
 * Needs to be executed after zl70550_init() but before the device is used.
 *
 * @ingroup ZL70550
 */
void zl70550_restore_state();

/**
 * Set operation frequency
 *
 * Sets the operating frequency to 869.7 Mhz.
 * Needs to be executed after zl70550_init() but before the device is used.
 *
 * @ingroup ZL70550
 */
void zl70550_set_frequency();

/**
 * Send test packet
 *
 * This function tests radio operation by transmitting two bytes 0x1 followed by
 * 0x2. CSMA is disabled for this test.
 *
 */
void zl70550_raw_transmit();

/**
 * Receive packet
 *
 * Function that can be used to receive a packet to a provided `rx_buffer`.
 * Additionally it can operate in a perpetual loop with the `forever` flag.
 * Hamming encoding is enabled.
 *
 * @param rx_buffer Output: Buffer in which the data will be stored.
 * @param forever Flag to signal a perpetual receive loop.
 *
 * @ingroup ZL70550
 */
void zl70550_packet_receive(uint8_t* rx_buffer, bool forever);

/**
 * Transmit a packet
 *
 * Transmits the data contained in the `data` parameter with length `length`.
 * CSMA is disabled and Hamming encoding is enabled.
 *
 * @param data Data to transmit
 * @param length Length of the `data` to transmit
 *
 * @ingroup ZL70550
 */
void zl70550_packet_transmit(uint8_t* data, uint16_t length);

/**
 * WhoAmI function to test SPI communication.
 *
 * @return Expected to be 0x47.
 */
uint8_t zl70550_whoAmI();

#endif /* INCLUDE_zl70550_H */
