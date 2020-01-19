#ifndef INCLUDE_PLATFORM_H
#define INCLUDE_PLATFORM_H

#include "configuration.h"

#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)

#if HWREV == 4
#include <botoks_mote-2-0.h>
#elif HWREV == 3
#include <botoks_mote-1-2.h>
#elif HWREV == 2
#elif HWREV == 1
#include "dev_board.h"
#else
#error "Please define a platform a hardware revision -DHWREV=X"
#endif

#if !defined(PROFILE) || !defined(PROFILE_RX_ERROR) || \
    !defined(TPSYNC_ENABLE) || !defined(TPSYNC_DELAY_TX_ENABLE)
#error \
    "Undefined options. One or more options undefined. Please specify all the options"
#endif

#endif /* INCLUDE_PLATFORM_H */
