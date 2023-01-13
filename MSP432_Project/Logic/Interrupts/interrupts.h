/*
 * interrupts.h
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

// GENERAL HEADER FILE FOR ALL HARDWARE INTERRUPTS

#ifndef LOGIC_INTERRUPTS_INTERRUPTS_H_
#define LOGIC_INTERRUPTS_INTERRUPTS_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>

// Graphics
#include "Hardware/Graphics/graphics_context.h"     //imported twice (present also in hardware_init.h) just to avoid problems in case of future changes in hardware_init.h
#include "Hardware/Graphics/direction_graphics.h"
#include "Hardware/Graphics/images_definitions.h"

// Bluetooth
#include "Hardware/Bluetooth/Controller/UART_IO.h"

// Logic of the project (all functions which drive the hardware based on decisions on variables values)
#include "Logic/project_logic.h"

// functions for timers testing
void test_timers_main();

extern int8_t mode_selected;

#endif /* LOGIC_INTERRUPTS_INTERRUPTS_H_ */
