/*
 * project_logic.h
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#ifndef LOGIC_PROJECT_LOGIC_H_
#define LOGIC_PROJECT_LOGIC_H_

#include "Hardware/Graphics/menu_graphics.h"        //used for Selection_t enumeration

// declaring global variables for current speed state and information about next command to be sent
extern int curr_val;
extern int forw_backw;
extern int right_left;

extern Selection_t currentSelection;

void _hwInit();
void start_graphics();
void start_menu();
void activate_peripherals();
void findCommand();
void ble_command_manager();
void drawSelection(int y);


#endif /* LOGIC_PROJECT_LOGIC_H_ */
