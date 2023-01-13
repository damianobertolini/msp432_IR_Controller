#ifndef HARDWARE_UART_DRIVER_H_
#define HARDWARE_UART_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/uart.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>


#define UARTA0_BUFFERSIZE 2048
#define UARTA2_BUFFERSIZE 2048

extern const eUSCI_UART_ConfigV1 UARTConfig;

// initializes UART
void UART_Init(uint32_t UART, eUSCI_UART_ConfigV1 UARTConfig);

// simple function to communicate directly with hardware, writes on UART buffer
void UART_Write(uint32_t UART, uint8_t *Data, uint32_t Size);

// simple function to communicate directly with hardware, reads from UART buffer
uint32_t UART_Read(uint32_t UART, uint8_t *Data, uint32_t Size);

// checks whether the UART is available (properly initialized)
uint32_t UART_Available(uint32_t UART);

// erases UART buffer
void UART_Flush(uint32_t UART);

#endif /* HARDWARE_UART_DRIVER_H_ */
