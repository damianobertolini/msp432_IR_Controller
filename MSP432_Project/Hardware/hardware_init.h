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


void _lowPowerInit();
void _PCM_Flash_WDT_Init();
void _ledInit();
void _timersInit();
void _buttonsInit();
void _joystickInit();
void _accelSensorInit();
void _adcInit();
void _bluetoothInit();
void _graphicsInit();


#endif /* HARDWAREINIT_H_ */
