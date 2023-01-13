/*
 * HardwareInit.h
 *
 *  Created on: 30 nov 2022
 *      Author: damiano
 */

#ifndef HARDWAREINIT_H_
#define HARDWAREINIT_H_

#include "Hardware/Graphics/graphics_context.h"

/* Timers periods (CCR0 values) */
#define TIMER_PERIOD3    0x05DC   //1500 to hex
#define TIMER_PERIOD2   0x03E8   //1000 to hex


//This function sets all pins of all ports to output mode and low value (some pins will be later overwritten by other Init functions) and should solve the warning of power consumption on unused pins
//EVEN IF the warning might still remain as this was reported to be a bug:
//see https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/535365/ulp4-1-issues-on-msp432 for further information
void _lowPowerInit();

// stops watchdog timer and initializes some flash memory features (e.g. Voltage)
void _PCM_Flash_WDT_Init();

// initializes pin on Port 1 Pin 0 as output pin to use the connected LED
void _ledInit();

// initializes the two timers used for graphics updating and IR signals sending
void _timersInit();

// initializes Buttons on TI J4.32 and J4.33 and Joystick input button
void _buttonsInit();

//initialize Analog to Digital Converter for Joystick and enables its interrupts
void _joystickInit();

//initialization of accelerometer (Pins and memory location where result of conversion has to be stored)
void _accelSensorInit();

//generic initialization of ADC14, to be used in conjunction with _adcInit() and/or _joystickInit()
void _adcInit();

//initialization of the Bluetooth connection through UART
void _bluetoothInit();

//initialize Liquid Crystal Display
void _graphicsInit();


#endif /* HARDWAREINIT_H_ */
