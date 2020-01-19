#ifndef INCLUDE_TPTIME_H
#define INCLUDE_TPTIME_H

#include <stdint.h>

/**
 * @defgroup tptime TPTime
 */

/**
 * @defgroup tptime-conf TPTime CHRT configuration
 */

/* Return codes. */
#define TPTIME_OK 0
#define TPTIME_OBJ_VALID 1
#define TPTIME_OBJ_EXPIRED 2
#define TPTIME_MEM_ERROR -1
#define TPTIME_OBJ_NOT_FOUND -2
#define TPTIME_TIME_PAST -3

/**
 * @def TPTIME_CHRT_START_TIER
 *
 * First CHRT tier to be recharged on first boot.
 *
 * @ingroup tptime-conf
 */
#define TPTIME_CHRT_START_TIER 0

/**
 * @def TPTIME_CHRT_MARGIN_UP
 *
 * Number of additional tiers, larger than the current one, to recharge.
 *
 * @ingroup tptime-conf
 */
#define TPTIME_CHRT_MARGIN_UP 3

/**
 * @def TPTIME_CHRT_MARGIN_DOWN
 *
 * Number of additional tiers, smaller than the current one, to recharge.
 *
 * @ingroup tptime-conf
 */
#define TPTIME_CHRT_MARGIN_DOWN 3

/**
 * @def TPTIME_CHRT_FIX_TIERS
 *
 * Whether to charge the same tier throughout the whole application.
 *
 * @ingroup tptime-conf
 */
#define TPTIME_CHRT_FIX_TIERS false

/**
 * Initialize timekeeping subsystem.
 *
 * This function inctrements system time at each reboot, querying the CHRT, and
 * recharges the CHRT as well.  It must be called at the beginning of the
 * `main()`.
 *
 * **Note**: The timekeeping subsystem uses an MCU timer with a specific clock
 * configuration, take this into account when designing your application.  For
 * the MSP430FR59xx, it is timer TA0 with SMCLK at 0.25 MHz
 *
 * @ingroup tptime
 */
void tptime_init(void);

/**
 * Get timestamp (current time) in milliseconds.
 *
 * @return system time in milliseconds
 *
 * @ingroup tptime
 */
uint32_t tptime_get_timestamp(void);

/**
 * Set expiration time, in milliseconds, for an arbitraty object.
 *
 * The expiration time is relative to the current system time (as would be
 * returned by `tptime_get_timestamp()`).  An object can be a variable, an
 * array, a function or any other arbitrary void pointer.  The expiration
 * status of the given object can be checked with `tptime_has_expired()`.
 *
 * **Note**: This function uses `malloc()` internally.  Expiration times are
 * not deallocated unless the user explicitely calls
 * `tptime_clear_expiration()`.
 *
 * @param tag        void pointer to object to set expiration time for
 * @param expiration expiration time, in milliseconds, relative to current time
 *
 * @return `TPTIME_OK` or `TPTIME_MEM_ERROR`
 *
 * @ingroup tptime
 */
int8_t tptime_set_expiration(void* tag, uint32_t expiration);

/**
 * Clear expiration time.
 *
 * An expiration time previously set with `tptime_set_expiration()` can be
 * deallocated using this function.
 *
 * @param tag void pointer to object to clear expiration time of
 *
 * @return `TPTIME_OK` or `TPTIME_OBJ_NOT_FOUND`
 *
 * @ingroup tptime
 */
int8_t tptime_clear_expiration(void* tag);

/**
 * Check if given object has expired.
 *
 * @param tag void pointer to object to check expiration of
 *
 * @return `TPTIME_OBJ_EXPIRED`, `TPTIME_OBJ_VALID` or `TPTIME_OBJ_NOT_FOUND`
 *
 * @ingroup tptime
 */
int8_t tptime_has_expired(void* tag);

#endif /* INCLUDE_TPTIME_H */
