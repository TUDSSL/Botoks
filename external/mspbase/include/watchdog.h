#ifndef INCLUDE_MSPBASE_WD_H
#define INCLUDE_MSPBASE_WD_H

/**
 * Disable watchdog timer.
 */
#define msp_watchdog_disable() WDTCTL = WDTPW | WDTHOLD

#endif /* INCLUDE_MSPBASE_WD_H */
