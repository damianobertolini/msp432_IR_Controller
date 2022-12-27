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
#define TIMER_PERIOD    0x0BB8   //4000 to hex
#define TIMER_PERIOD2   0x07D0   //6000 to hex
#define TIMER_PERIOD1   0x03E8   //1000 to hex


void _lowPowerInit();
void _PCM_Flash_Init();
void _ledInit();
void _timersInit();
void _buttonsInit();
void _joystickInit();
void _accelSensorInit();
void _adcInit();
void _bluetoothInit();
void _graphicsInit();


#endif /* HARDWAREINIT_H_ */
