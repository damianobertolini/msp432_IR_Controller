/*
 * HardwareInit.h
 *
 *  Created on: 30 nov 2022
 *      Author: damiano
 */

#ifndef HARDWAREINIT_H_
#define HARDWAREINIT_H_


/* Graphic library context */
Graphics_Context g_sContext;

void _lowPowerInit();
void _PCM_Flash_Init();
void _ledInit();
void _timersInit();
void _buttonsInit();
void _joystickInit();
void _accelSensorInit();
void _adcInit();
void customDelay();
void _graphicsInit();
void startImageHelicopter();

//Screen_t currentScreen = HELICOPTER_GRAPHICS;

#endif /* HARDWAREINIT_H_ */
