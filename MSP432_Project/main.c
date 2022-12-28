/*
 * main.c
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>

#include <stdio.h>

#include "Logic/project_logic.h"
#include "Hardware/Graphics/direction_graphics.h"

// for debugging and testing
#include "Logic/test.h"

void main(void)
{
    // initializes all the hardware
    _hwInit();

    // draws helicopter startup image
    start_graphics();

    // draws the menu
    start_menu();


    // enables Interrupt master and start first clock, in order to start getting buttons, timer and adc interrupts
    activate_peripherals();

    if(TEST){
        // Interrupts need to be enabled
        test_timers_main();

        // testing if IR is sending the correct signals
        testCommands();

        // testing if Bluetooth manager function is correctly working
        testBluetooth();

        // testing graphics
        testDirectionGraphics1();
        testDirectionGraphics2();
    }

    while(1)
    {
        // MSP432 goes into sleep mode until next interrupt is fired
        PCM_gotoLPM0();
    }
}






