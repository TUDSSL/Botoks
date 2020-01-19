#ifndef INCLUDE_CHRT_H
#define INCLUDE_CHRT_H

#include <stdbool.h>
#include <stdint.h>

/* Resolution of the ADC. */
#define ADC_RESOLUTION                  12

/* Calibration table length. */
#define TABLE_LEN                       (1 << ADC_RESOLUTION)

/* MCLK frequency, in MHz. */
#define MCLK_FREQ_MHZ                   8

/* Microseconds in a tick of calibration timer, resulting from SMCLK frequency
 * (0.5 MHz) and timer divider (48). */
#define US_PER_TICK                     96u

/* Number of cycles to delay inside delay loops. */
#define DELAY_CYCLES                    2

/* Return codes. */
#define TK_SUCCESS                      0
#define TK_UNKNOWN_ID                   1
#define TK_ERROR                        2

/* Number of CHRT tiers in the design. */
#define N_TIERS                         4

/* Resolution of the CHRT tiers in ms. */
#define TIER_0_RESOLUTION               1
#define TIER_1_RESOLUTION               10
#define TIER_2_RESOLUTION               100
#define TIER_3_RESOLUTION               1000

/* User configuration
 *
 * These parameters are crucial to the accuracy of the CHRT and need to be
 * calibrated for each device.
 */

/**
 * Defines the transition point between two tiers, defined in ADC steps. That
 * is, the point at which the comparator trips.
 *
 * @ingroup CHRT_CONF
 */
#define CASCADE_TRIGGER_THRESHOLD 410

/** @defgroup CHRT_CASCONF Cascade time compensation
 *
 * These constants define the total time of the tier used as compensation when
 * sampling multiple tiers.
 * @{
 */
/** Defined in the resolution of tier 1. */
#define TIER_TOTAL_TIME_0 8
/** Defined in the resolution of tier 2. */
#define TIER_TOTAL_TIME_1 8
/** Defined in the resolution of tier 3. */
#define TIER_TOTAL_TIME_2 8
/** @} */

/**
 * @defgroup CHRT_DISCONF Discharge time
 *
 * Sets the delay required to fully discharge a specific tier. Defined in loops
 * of `DELAY_CYCLES`
 * @{
 */
#define TIER_DISCHARGE_TIME_0 50
#define TIER_DISCHARGE_TIME_1 50
#define TIER_DISCHARGE_TIME_2 200
#define TIER_DISCHARGE_TIME_3 700
/** @} */

/**
 * @defgroup CHRT_CHRCONF Charge time calibration
 *
 * Sets the delay required to fully charge a specific tier. Defined in loops of
 * `DELAY_CYCLES`
 * @{
 */
#define TIER_CHARGE_TIME_0 22
#define TIER_CHARGE_TIME_1 74
#define TIER_CHARGE_TIME_2 26
#define TIER_CHARGE_TIME_3 90
/** @} */


/**
 * @defgroup CHRT_CHRCMP Charge time compensation
 *
 * Sets the optionally applied charge time compensation for tier 0.
 * (Charge time is only significant to tiers 0 and 1)
 * Defined in ms.
 *
 * @{
 */
#define TIER_CHARGE_TIME_0_MS 0
#define TIER_CHARGE_TIME_1_MS 1
#define TIER_CHARGE_TIME_2_MS 1
#define TIER_CHARGE_TIME_3_MS 3
/** @} */

/**
 * @defgroup CHRT_LVLCHANGE Change target tier threshold
 *
 * Sets the threshold at which a lower/higher tier will be selected at next
 * charging cycle. Defined in the tiers scale.
 *
 * @{
 */
#define TIER_CHANGE_LEVEL_LOW 10
#define TIER_CHANGE_LEVEL_HIGH 90
/** @} */

/* End of user configuration */

/**
 * @defgroup CHRT CHRT
 */

/**
 * Charge a single tier of the CHRT.  *Only for debugging*.
 *
 * @param tk_id tier to charge
 *
 * @return `TK_SUCCESS`, or `TK_UNKNOWN_ID` when called with invalid `tk_id`
 */
uint8_t chrt_charge_tier(uint8_t tk_id);

/**
 * Start discharge of a specific tier.  *Only for debugging*.
 *
 * @param tk_id tier to charge
 *
 * @return `TK_SUCCESS`, or `TK_UNKNOWN_ID` when called with invalid `tk_id`
 */
uint8_t chrt_start_discharge(uint8_t tk_id);

/**
 * Initialize the CHRT.
 *
 * Configure I/O and initializes non-volatile variables such as calibration
 * constants on first boot after programming.
 *
 * @param start_tier first CHRT tier to be charged on first boot
 * @param KR         number of additional tiers larger than the current one to
 *                   recharge
 * @param KL         number of additional tiers smaller than the current one to
 *                   recharge
 * @param fix_tiers  whether to charge the same tier (`start_tier`) throughout
 *                   the whole application (*only for debugging*)
 *
 * @return `TK_SUCCESS`
 *
 * Smaller values of KR and KL minimize energy consumption, as fewer tiers are
 * recharged each time.  Larger values minimize timekeeping errors when timed
 * intervals vary more quickly.
 *
 * @ingroup CHRT
 */
uint8_t chrt_init(uint8_t start_tier, uint8_t KR, uint8_t KL, bool fix_tiers);

/**
 * Charge the CHRT.
 *
 * Depending on the last on-off period, the current (optimal) tier will change
 * when the last measurement was near the limits of the current tier.  This
 * combined with the margins specified in chrt_init() ensure that the CHRT
 * follows changes in energy availability.
 *
 * @return `TK_SUCCESS`
 *
 * @ingroup CHRT
 */
uint8_t chrt_charge();

/**
 * Samples a single tier of the CHRT.  *Only for debugging*.
 *
 * @param tk_id      tier to sample
 * @param resolution measurement resolution in ms
 * @param time       time elapsed since last recharge
 * @param debug      enable print of the sampled ADC value (*only for
 *                   debugging*)
 *
 * @return `TK_SUCCESS`
 *
 * To obtain time in millisecond, multiply `time` by `resolution`.
 */
uint8_t chrt_sample_tier(uint8_t tk_id, uint16_t* resolution, uint16_t* time,
                         bool debug);

/**
 * Retrieve time from the CHRT.
 *
 * Depending on the margin specified in chrt_init(), this function samples the
 * tiers to retrieve a notion of time.
 *
 * @param resolution   measurement resolution in ms
 * @param time         time elapsed since last recharge
 * @param cmp_chr_time when true compensates the timing output for charging times
 *
 * @return `TK_SUCCESS`, or `TK_ERROR` when operating outside of the CHRT optimal
 *         timing range
 *
 * To obtain time in millisecond, multiply time by resolution.
 *
 * @ingroup CHRT
 */
uint8_t chrt_get_time(uint16_t* resolution, uint16_t* time, bool cmp_chr_time);

#endif /* INCLUDE_CHRT_H */
