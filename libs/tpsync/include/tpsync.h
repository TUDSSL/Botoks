#ifndef INCLUDE_TPSYNC_H
#define INCLUDE_TPSYNC_H

#include <stdint.h>

/**
 * @defgroup tpsync TPSync
 */

/**
 * @defgroup tpsync-sync-err TPSync sync error
 */

/**
 * @defgroup tpsync-conf TPSync configuration
 */

/* Return codes. */
#define TPSYNC_SUCCESS 0
#define TPSYNC_NULL_DELAY 1
#define TPSYNC_NO_RX_SUCCESS -1
#define TPSYNC_NULL_TX_WUP -2

/**
 * @def AVG_WINDOW
 *
 * Number of power cycles to use when computing the average wake-up period.
 *
 * @ingroup tpsync-conf
 */
#define AVG_WINDOW 4u

/**
 * @def ERROR_CORR_COEFF
 *
 * Controls the scaling factor for error synchronization (use with
 * track_sync_error_start and track_sync_error_stop).
 *
 * @ingroup tpsync-conf
 */
#define ERROR_CORR_COEFF 1u  // experimentally tuned

/**
 * @def SYNC_SLACK_MS
 *
 * Controls how much the receiver wakes up ahead of schedule. A
 * larger `SYNC_SLACK_MS` allows for more synchronization error, but increases
 * energy consumption, and consequently the chance of missing a packet.
 *
 * @ingroup tpsync-conf
 */
#define SYNC_SLACK_MS 2u  // experimentally tuned

/* For uncalibrated time intervals. */
#define INFINITE_TIME 0xFFFF

/**
 * Compute average wake-up period over a sliding window.
 *
 * @param last_period duration of the last wake-up cycle
 *
 * @return average of last `AVG_WINDOW` periods
 *
 * This function calculates the average over a sliding window of the last
 * wake-up periods.
 *
 * @see `AVG_WINDOW`
 *
 * @ingroup tpsync
 */
uint16_t get_avg_wakeup_period(uint16_t last_period);

/**
 * Delay radio transmission to align to synchronized schedule.
 *
 * @param tx_wakeup_period duration of the last wake-up cycle
 *
 * @return `TPSYNC_NULL_DELAY` in case the transmission is not delayed,
 *         `TPSYNC_SUCCESS` otherwise
 *
 * The delay is implemented as a low-power mode (LPM1) sleep interrupted by a
 * timeout. SMCLK and TA1 are used to implement the timeout.
 */
int delay_radio_transmission(uint16_t tx_wakeup_period);

/**
 * Delay radio reception to aligh to synchronized schedule.
 *
 * @param rx_wakeup_period duration of the last wake-up cycle
 * @param tx_wakeup_period duration of the last wake-up cycle of the
 *                         transmitting node
 * @param error            nominal timekeeping error
 *
 * @return `TPSYNC_NULL_TX_WUP` in case the wake-up cycle of the transmitting
 *         node has null duration, `TPSYNC_NULL_DELAY` in case reception is not
 *         delayed, `TPSYNC_NO_RX_SUCCESS` in case a packed was not received
 *         (and reception is not delayed), `TPSYNC_SUCCESS` otherwise
 *
 * The delay is implemented as a low-power mode (LPM1) sleep interrupted by a
 * timeout. SMCLK and TA1 are used to implement the timeout.
 *
 * @see `SYNC_SLACK_MS`
 *
 * @ingroup tpsync
 */
int delay_radio_listening(uint16_t rx_wakeup_period, uint16_t tx_wakeup_period,
                          uint16_t error);

/**
 * Start tracking synchronization error.
 *
 * This function can be called when the receiving node wakes up to start
 * tracking how much time is wasted waiting for a packet with the radio on.
 *
 * @see `track_sync_error_stop()`
 * @see `ERROR_CORR_COEFF`
 *
 * @ingroup tpsync-sync-err
 */
void track_sync_error_start(void);

/**
 * Stop tracking synchronization error.
 *
 * This function can be called upon reception of a packet to compute the total
 * idle listening time. This information can be used in a subsequent radio cycle
 * as a feedback control to minimize idle listening. `ERROR_CORR_COEFF` can be
 * used to scale down the magnitude of the synchronization error, effectively
 * working as the P parameter in a proportional feedback controller.
 *
 * @ingroup tpsync-sync-err
 */
void track_sync_error_stop(void);

#endif /* INCLUDE_TPSYNC_H */
