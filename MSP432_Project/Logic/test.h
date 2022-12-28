/*
 * test.h
 *
 *  Created on: 28 dic 2022
 *      Author: damiano
 */

#ifndef LOGIC_TEST_H_
#define LOGIC_TEST_H_

#include "Logic/Interrupts/interrupts.h"
#include "Logic/project_logic.h"
#include "Hardware/Graphics/direction_graphics.h"


#define TEST 0  // set to one to begin test session

extern int test_command;    // set to 0 to get the fully functional helicopter controller

extern int n_loops_timer_id;    // used for timers test, this variable could be local to the timers_interrupt source file but was put here just to show it has a testing purpose


#endif /* LOGIC_TEST_H_ */
