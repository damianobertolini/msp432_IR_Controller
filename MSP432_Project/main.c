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


void main(void)
{
    _hwInit();

    //startupHelicopter()
    //startMenu

    // send a command
    while(1)
    {
        // MSP432 goes into sleep mode until next interrupt is fired
        PCM_gotoLPM0();
    }
}


void test1()
{
    Interrupt_disableMaster();
    ADC14_disableConversion();

    curr_val = 0;
    forw_backw = 1;
    right_left = 0;

    findCommand();

    forw_backw = -1;
    right_left = 0;

    findCommand();

    forw_backw = 0;
    right_left = -1;

    findCommand();

    forw_backw = 0;
    right_left = -1;
    findCommand();
}
