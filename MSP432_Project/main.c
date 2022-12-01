/*
 * main.c
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

#include "CommandMatrices.h"
#include "SendCodes_main.h"
#include "HardwareInit.h"



void _hwInit()
{
    //sets core voltage level and number of wait states used by flash controller for read operation
    _PCM_Flash_Init();

    //initialize LED on Pin 1.0
    _ledInit();

    //initialize TimerA2 and TimerA3 into upMode
    _timersInit();

    //initialize buttons S1 and S2 (pins J4.32 and J4.33) on BoosterPack Module
    _buttonsInit();

    //initialize ADC
    _adcInit();

    //initialize LCD
    _graphicsInit();

    //initialize hardware for PWM and IR emitter
    _initPWM();

    //enable master interrupt
    Interrupt_enableMaster();

    //start first timer (TIMER_A3)
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
}




void main(void)
{
    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // call all initialization functions
    _hwInit();


    // send a command
    while(1)
    {
    }

}

//global variables for current speed state and information about next command to be sent
int curr_val = 0;
int forw_backw = 0;
int right_left = 0;


//finds the right command to send and calls sendCommand to send it
findCommand()
{
    //create in-function local copy of global variables in order to have the possibility to erase their value
    int curr_forw_backw = forw_backw;
    int curr_right_left = right_left;

    //erase value (user will have to trigger again the device (Joystick, accelerometer, ...) in order to put them back to 1
    forw_backw = 0;
    right_left = 0;

    //forward or backward
    if(curr_forw_backw == 1)
    {
        sendCommand(fw_matrix[curr_val], fw_matrix_p[curr_val], sizeof(fw_matrix[curr_val]) / sizeof(fw_matrix[curr_val][0]));
        return;
    }
    if(curr_forw_backw == -1)
    {
        sendCommand(bk_matrix[curr_val], bk_matrix_p[curr_val], sizeof(bk_matrix[curr_val]) / sizeof(bk_matrix[curr_val][0]));
        puts("CI");

        return;
    }

    //right or left
    if(curr_right_left == 1)
    {
        sendCommand(right_matrix[curr_val], right_matrix_p[curr_val], sizeof(right_matrix[curr_val]) / sizeof(right_matrix[curr_val][0]));
        return;
    }
    if(curr_right_left == -1)
    {
        sendCommand(left_matrix[curr_val], left_matrix_p[curr_val], sizeof(left_matrix[curr_val]) / sizeof(left_matrix[curr_val][0]));
        return;
    }

    //default maintain current propeller speed
    sendCommand(up_matrix[curr_val], up_matrix_p[curr_val], sizeof(up_matrix[curr_val]) / sizeof(up_matrix[curr_val][0]));

}


//HARDWARE INTERRUPTS



//The two Timers alternate in triggering interrupts; this is necessary as the real remote control emits IR signals with different time intervals
//Note I'm not calling findCommand() inside the TIMER Interrupts (ex. after ADC14_toggleConversionTrigger()); as this function is called AT THE END of the timer interrupt
//This would mean not getting a responsive device.

//Timer A2 Capture/Compare Register 0 Interrupt handler
void TA2_0_IRQHandler(void)
{
        //trigger ADC conversion of Joystick position to get next command to send
        ADC14_toggleConversionTrigger();


        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);

        //stopping timer A2 and starting timer A3 -> sending signals with different time intervals
        Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
        Timer_A_stopTimer(TIMER_A2_BASE);
}

//Timer A3 Capture/Compare Register 0 Interrupt handler
void TA3_0_IRQHandler(void)
{
        //trigger ADC conversion of Joystick position to get next command to send
        ADC14_toggleConversionTrigger();


        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);

        //stopping timer A3 and starting timer A2 -> sending signals with different time intervals
        Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
        Timer_A_stopTimer(TIMER_A3_BASE);
}


static uint16_t resultsBuffer[2];

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    /* ADC results buffer */
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);


    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        char string[10];
        sprintf(string, "X: %5d", resultsBuffer[0]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        50,
                                        OPAQUE_TEXT);


        //manipulate Joystick X position and decide accordingly whether to turn left, right or stay still
        right_left = 0; //reset right-left state (stop turning)

        if(resultsBuffer[0] > 14000)
        {
            right_left = 1;  //right
        }
        if(resultsBuffer[0] < 2000)
        {
            right_left = -1;  //left
        }


        sprintf(string, "Y: %5d", resultsBuffer[1]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        70,
                                        OPAQUE_TEXT);

        //manipulate Joystick Y position and decide accordingly whether to move forward, backward or stay still
        forw_backw = 0;  //reset forw-backw state (stop moving)

        if(resultsBuffer[1] > 14000)
        {
            forw_backw = 1;  //forward
        }
        if(resultsBuffer[1] < 2000)
        {
            forw_backw = -1; //backward
        }


        //decide which command to send and emit IR signals accordingly
        findCommand();

        //toggle LED state in order to notify the user that a command has been sent (mostly useful for debugging purposes)
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}



//Button S2 on BoosterPack Module
void PORT3_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    /* check if we received P3.5 interrupt */
    if((status & GPIO_PIN5)){
        if(curr_val == 0)
        {
            //won't decrement as velocity matrix index can't go under 0 (vector[-1] doesn't exist)
        }
        {
            //decrease speed
            curr_val--;
        }
    }
}


//Button S1 on BoosterPack Module
void PORT5_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    /* check if we received P5.1 interrupt */
    if((status & GPIO_PIN1)){
        if(curr_val == 4)
        {
            //won't increment as velocity matrix only has 5 possible positions
        }
        else
        {
            //increase speed
            curr_val++;
        }
    }
}
