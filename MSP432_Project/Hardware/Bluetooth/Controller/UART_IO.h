#ifndef MSPIO_H_
#define MSPIO_H_

#include <stdio.h>

#include "Hardware/Bluetooth/Hardware/UART_Driver.h"

#define BLUETOOTH_BUFFER_SIZE    1    //maximum number of character that the uart bluetooth buffer can store

extern char Buffer[BLUETOOTH_BUFFER_SIZE];  //Buffer which will make a copy of the default buffer (UARTA0Data) where UART data for Bluetooth are stored


void MSPrintf(uint32_t UART, const char *fs, ...);
int MSPgets(uint32_t UART, char *b, int size);
int customMSPgets(uint32_t UART, char *b, int size);

#endif /* MSPIO_H_ */
