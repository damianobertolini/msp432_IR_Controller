/*
 * main.c
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#include <CommandMatrices.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

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


    ADC14_toggleConversionTrigger();

    // send a command
    while(1)
    {
    }

}


int curr_val = 0;
int curr_forw_backw = 0;
int curr_des_sin = 0;


void TA3_0_IRQHandler(void)
{

        //sendCommand(av_5, av_5_p);
        if(curr_forw_backw == 1)
        {
            sendCommand(fw_matrix[curr_val], fw_matrix_p[curr_val], sizeof(fw_matrix[curr_val]) / sizeof(fw_matrix[curr_val][0]));
        }
        else
        {
            if(curr_forw_backw == -1)
            {
                sendCommand(bk_matrix[curr_val], bk_matrix_p[curr_val], sizeof(bk_matrix[curr_val]) / sizeof(bk_matrix[curr_val][0]));
            }
            else
            {
                if(curr_des_sin == 1)
                {
                    sendCommand(right_matrix[curr_val], right_matrix_p[curr_val], sizeof(right_matrix[curr_val]) / sizeof(right_matrix[curr_val][0]));
                }
                else
                {
                    if(curr_des_sin == -1)
                    {
                        sendCommand(left_matrix[curr_val], left_matrix_p[curr_val], sizeof(left_matrix[curr_val]) / sizeof(left_matrix[curr_val][0]));
                    }
                    else
                    {
                        //default
                        sendCommand(up_matrix[curr_val], up_matrix_p[curr_val], sizeof(up_matrix[curr_val]) / sizeof(up_matrix[curr_val][0]));
                    }
                }
            }
        }

        curr_forw_backw = 0;
        curr_des_sin = 0;


        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);


        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);

        Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
        Timer_A_stopTimer(TIMER_A3_BASE);


        ADC14_toggleConversionTrigger();
}


void TA2_0_IRQHandler(void)
{

        //sendCommand(av_5, av_5_p);
        if(curr_forw_backw == 1)
        {
            sendCommand(fw_matrix[curr_val], fw_matrix_p[curr_val], sizeof(fw_matrix[curr_val]) / sizeof(fw_matrix[curr_val][0]));
        }
        else
        {
            if(curr_forw_backw == -1)
            {
                sendCommand(bk_matrix[curr_val], bk_matrix_p[curr_val], sizeof(bk_matrix[curr_val]) / sizeof(bk_matrix[curr_val][0]));
            }
            else
            {
                if(curr_des_sin == 1)
                {
                    sendCommand(right_matrix[curr_val], right_matrix_p[curr_val], sizeof(right_matrix[curr_val]) / sizeof(right_matrix[curr_val][0]));
                }
                else
                {
                    if(curr_des_sin == -1)
                    {
                        sendCommand(left_matrix[curr_val], left_matrix_p[curr_val], sizeof(left_matrix[curr_val]) / sizeof(left_matrix[curr_val][0]));
                    }
                    else
                    {
                        //default
                        sendCommand(up_matrix[curr_val], up_matrix_p[curr_val], sizeof(up_matrix[curr_val]) / sizeof(up_matrix[curr_val][0]));
                    }
                }
            }
        }

        curr_forw_backw = 0;
        curr_des_sin = 0;


        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);


        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);

        Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
        Timer_A_stopTimer(TIMER_A2_BASE);


        ADC14_toggleConversionTrigger();
}



/* ADC results buffer */
static uint16_t resultsBuffer[2];


/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
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

        if(resultsBuffer[0] > 14000)
        {
            curr_des_sin = 1;  //des
        }
        else
        {
            if(resultsBuffer[0] < 2000)
            {
                curr_des_sin = -1;  //sin
            }
            else
            {
                curr_des_sin = 0;
            }
        }


        sprintf(string, "Y: %5d", resultsBuffer[1]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        70,
                                        OPAQUE_TEXT);

        if(resultsBuffer[1] > 14000)
        {
            curr_forw_backw = 1;  //forward
        }
        else
        {
            if(resultsBuffer[1] < 2000)
            {
                curr_forw_backw = -1;  //backward
            }
            else
            {
                curr_forw_backw = 0;  //reset forw-backw state (stop moving)
            }
        }


        /* Determine if JoyStick button is pressed */
        int buttonPressed = 0;
        if (!(P4IN & GPIO_PIN1))
            buttonPressed = 1;

        sprintf(string, "Button: %d", buttonPressed);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        AUTO_STRING_LENGTH,
                                        64,
                                        90,
                                        OPAQUE_TEXT);
    }
}



//Button S2 on BoosterPack Module
void PORT3_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN5)){
        /* Toggling the output on the LED */


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
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN1)){
        /* Toggling the output on the LED */

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
