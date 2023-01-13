/*
 * project_logic.h
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#ifndef LOGIC_PROJECT_LOGIC_H_
#define LOGIC_PROJECT_LOGIC_H_

#include "Hardware/Graphics/menu_graphics.h"        //used for Selection_t enumeration

#include <stdint.h>     //for int8_t and int64_t definition

// declaring global variables for current speed state and information about next command to be sent
extern uint8_t curr_val;
extern int8_t forw_backw;
extern int8_t right_left;

extern Selection_t currentSelection;

//initializes all hardware
void _hwInit();

// start the first helicopter image at the startup
void start_graphics();

// draws the menu
void start_menu();

// activates all peripherals interrupts and start the first of the two timers which manage the IR emission
void activate_peripherals();

// finds the right command to send based on forw_backw and right_left and calls sendCommand() to actually send it through PWM
void findCommand();

// this functions implements the logical control of the Bluetooth module, interpreting received values and taking the appropriate actions
void ble_command_manager();

// move between the three menu options and redraws the option images based on the current selection
// the upper and lower bound for the y value received are set to 9800 and 7000 in order to ignore little and unwanted movements
// inputs: current value of Joystick y - Axis
void drawSelection(uint64_t y);

// tests if commands sent are correct based on curr_val, forw_backw and right_left values
void testCommands();

// tests if Bluetooth commands sent are the correct ones based on Buffer value
void testBluetooth();


#endif /* LOGIC_PROJECT_LOGIC_H_ */
