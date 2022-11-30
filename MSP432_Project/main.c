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

#include "SendCodes_main.h"

#include "command_matrices.h"



unsigned int forward[34] =                {1950,300,300,250,400,400,250,400,450,350,350,400,400,350,450,400,350,200,250,250,400,300,250,250,450,400,300,250,400,200,400,400,250, 250};
unsigned int forward_p[34] =              {1950,200,750,200,200,700,200,200,200,200,200,200,200,200,200,200,750,800,700,200,700,700,700,200,200,200,750,200,750,200,200,750,700, 1000};

unsigned int des_1[34] =                  {1900,250,250,350,400,250,300,300,300,300,250,250,250,200,250,200,250,250,200,300,350,250,250,300,250,250,400,200,400,250,350,300,250, 300};
unsigned int des_1_p[34] =                {2000,300,350,150,700,300,250,300,300,300,350,750,750,800,700,750,750,750,750,200,350,750,700,650,350,200,750,200,750,300,200,750,750, 1000};


unsigned int backward[34] =               {1950,300,350,300,250,300,300,250,300,300,350,300,200,250,250,250,200,250,250,250,250,300,250,250,350,350,250,250,250,350,300,300,250, 200};
unsigned int backward_p[34] =             {1950,200,700,300,300,700,300,300,350,200,700,700,750,750,700,400,750,700,750,300,350,300,750,700,150,350,750,700,300,200,350,700,750, 1000};

unsigned int sin_1[34] =                  {1950,250,250,250,250,250,350,250,250,300,350,400,300,300,300,250,250,300,250,250,400,250,250,250,300,300,350,250,250,250,300,350,250, 300};
unsigned int sin_1_p[34] =                {1950,300,750,750,750,200,750,700,750,200,200,700,700,650,700,750,700,700,700,200,750,700,750,300,300,200,750,350,700,350,200,700,750, 1000};




/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

const Timer_A_UpModeConfig upConfig2 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD2,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};


const Timer_A_ContinuousModeConfig continuousModeConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,           // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,      // ACLK/1 = 32.768khz
        TIMER_A_TAIE_INTERRUPT_ENABLE,      // Enable Overflow ISR
        TIMER_A_DO_CLEAR                    // Clear Counter
};



void incl()
{

        /* P1.1 as input for button */
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
        GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);


        Interrupt_enableInterrupt(INT_PORT1);
        /* activate interrupt notification */


        /* Configuring P1.0 as output */
        GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

        /* Configuring Timer_A1 for Up Mode */
        Timer_A_configureUpMode(TIMER_A3_BASE, &upConfig);
        Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig2);

        /* Enabling interrupts and starting the timer */
        //Interrupt_enableSleepOnIsrExit();
        Interrupt_enableInterrupt(INT_TA3_0);
        Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
        Interrupt_enableInterrupt(INT_TA2_0);


        /* Enabling MASTER interrupts */
        Interrupt_enableMaster();
}






/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer[2];

void _adcInit(){
    /* Configures Pin 6.0 and 4.4 as ADC input */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

        /* Initializing ADC (ADCOSC/64/8) */
        ADC14_enableModule();
        ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

        /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
             * with internal 2.5v reference */
        ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
        ADC14_configureConversionMemory(ADC_MEM0,
                ADC_VREFPOS_AVCC_VREFNEG_VSS,
                ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

        ADC14_configureConversionMemory(ADC_MEM1,
                ADC_VREFPOS_AVCC_VREFNEG_VSS,
                ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

        /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
         *  is complete and enabling conversions */
        ADC14_enableInterrupt(ADC_INT1);

        /* Enabling Interrupts */
        Interrupt_enableInterrupt(INT_ADC14);
        Interrupt_enableMaster();

        /* Setting up the sample timer to automatically step through the sequence
         * convert.
         */
        ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

        /* Triggering the start of the sample */
        ADC14_enableConversion();
        //ADC14_toggleConversionTrigger();
}

void _graphicsInit()
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
        Graphics_clearDisplay(&g_sContext);

        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)"Joystick:",
                                        AUTO_STRING_LENGTH,
                                        64,
                                        30,
                                        OPAQUE_TEXT);

}

void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    //Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
   /* CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);*/

    _graphicsInit();
    _adcInit();
}



//BUTTONS on TI J4.32 and J4.33
void buttonsInit()
{
    //BUTTON S2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);


    Interrupt_enableInterrupt(INT_PORT3);


    //BUTTON S1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);


    Interrupt_enableInterrupt(INT_PORT5);


    Interrupt_enableMaster();

}



void main(void)
{
    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    // call all initialization functions
    initAll();


    incl();


    _hwInit();

    buttonsInit();


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

/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// main.c - MSP-EXP432P401R + Educational Boosterpack MkII - Joystick
//
//          Displays raw 14-bit ADC measurements for X/Y axis of Joystick
//
//****************************************************************************




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





void PORT1_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN1)){
        /* Toggling the output on the LED */

        curr_val++;

        if(curr_val == 5)
        {
            curr_val = 0;
        }
    }
}


void PORT3_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN5)){
        /* Toggling the output on the LED */

        curr_val--;

        if(curr_val == -1)
        {
            curr_val = 0;
        }
    }
}


void PORT5_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN1)){
        /* Toggling the output on the LED */

        curr_val++;

        if(curr_val == 5)
        {
            curr_val = 4;
        }
    }
}






