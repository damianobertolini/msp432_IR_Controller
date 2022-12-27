/*
 * direction_graphics.h
 *
 *  Created on: 14 dic 2022
 *      Author: damiano
 */

#ifndef MENU_GRAPHICS_H_
#define MENU_GRAPHICS_H_

typedef enum {JOYSTICK, ACCELEROMETER, BLUETOOTH} Selection_t;


void drawSelectionJoystick();
void drawSelectionAccelerometer();
void drawSelectionBluetooth();
void drawMenu();
void customDelay(int CYCLES);
void startImageHelicopter();


#endif /* MENU_GRAPHICS_H_ */
