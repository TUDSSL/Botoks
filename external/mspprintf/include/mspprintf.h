#ifndef INCLUDE_MSPPRINTF_H
#define INCLUDE_MSPPRINTF_H

#include <stdbool.h>
//#include <mspReseter.h>

int msp_printf(const char* format, ...);

void msp_uart_open();

void msp_uart_close();

#endif /* INCLUDE_MSPPRINTF_H */
