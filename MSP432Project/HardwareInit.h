/*
 * HardwareInit.h
 *
 *  Created on: 30 nov 2022
 *      Author: damiano
 */

#ifndef HARDWAREINIT_H_
#define HARDWAREINIT_H_


#define BLUETOOTH_BUFFER_SIZE    1    //maximum number of character that the uart bluetooth buffer can store

extern char Buffer[BLUETOOTH_BUFFER_SIZE];


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
void _bluetoothInit();
void customDelay();
void _graphicsInit();
void startImageHelicopter();

//Screen_t currentScreen = HELICOPTER_GRAPHICS;

#endif /* HARDWAREINIT_H_ */
