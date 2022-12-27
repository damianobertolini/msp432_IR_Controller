/*
 * project_logic.h
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#ifndef LOGIC_PROJECT_LOGIC_H_
#define LOGIC_PROJECT_LOGIC_H_

typedef enum {JOYSTICK, ACCELEROMETER, BLUETOOTH} Selection_t;

//global variables for current speed state and information about next command to be sent
extern int curr_val;
extern int forw_backw;
extern int right_left;

extern Selection_t currentSelection;


void _hwInit();
void findCommand();
void ble_command_manager();
void drawSelection(int y);
void start_graphics();
void start_menu();
void activate_peripherals();


#endif /* LOGIC_PROJECT_LOGIC_H_ */
