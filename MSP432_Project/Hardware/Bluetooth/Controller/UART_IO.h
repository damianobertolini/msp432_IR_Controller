#ifndef MSPIO_H_
#define MSPIO_H_

#include <stdio.h>

#include "Hardware/Bluetooth/Hardware/UART_Driver.h"

#define BLUETOOTH_BUFFER_SIZE    1    //maximum number of character that the uart bluetooth buffer can store

extern char Buffer[BLUETOOTH_BUFFER_SIZE];  //Buffer which will make a copy of the default buffer (UARTA0Data) where UART data for Bluetooth are stored

/*A basic printf for the MSP432. In order to use it properly you need to initialize the correct UART peripheral.
 * The following formats are supported:
 * %c = for char variables
 * %s = for string variables
 * %i = for unsigned integers
 * inputs: EUSCI_A0_BASE, "Formated string %c, %s, %i", character, string, integer*/
void MSPrintf(uint32_t UART, const char *fs, ...);

/*A basic gets for the MSP432. In order to use it properly you need to initialize the correct UART peripheral.
 * inputs: EUSCI_A0_BASE, InputBuffer, SizeOfInputBuffer*
 * outputs: number of characters read*/
int64_t MSPgets(uint32_t UART, char *b, int64_t size);

/*A basic non-blocking gets for the MSP432. In order to use it properly you need to initialize the correct UART peripheral.
 * inputs: EUSCI_A0_BASE, InputBuffer, SizeOfInputBuffer*
 * outputs: number of characters read*/
int64_t customMSPgets(uint32_t UART, char *b, int64_t size);

#endif /* MSPIO_H_ */
