/*
 * direction_graphics.h
 *
 *  Created on: 14 dic 2022
 *      Author: damiano
 */

#ifndef MENU_GRAPHICS_H_
#define MENU_GRAPHICS_H_

#include <stdint.h>

typedef enum {JOYSTICK, ACCELEROMETER, BLUETOOTH} Selection_t;

// redraw option images when JOYSTICK is selected
void drawSelectionJoystick();

// redraw option images when ACCELEROMETER is selected
void drawSelectionAccelerometer();

// redraw option images when BLUETOOTH is selected
void drawSelectionBluetooth();

// draws menu window
void drawMenu();

// delay using a counter
// inputs: value the counter needs to be equal to in order to exit the delay-loop
void customDelay(uint64_t CYCLES);

// draw the helicopter image at the startup of the system, with moving propellers at the end draws the menu
void startImageHelicopter();


#endif /* MENU_GRAPHICS_H_ */
