/*
 * main.c
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
<<<<<<< HEAD

=======
>>>>>>> nuovobuggy
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

<<<<<<< HEAD

#define MAX_PULSE 34
static Graphics_Context g_sContext;


/*
    SendCodes_main.c
    Author: Sam Vivacqua
    Last change: 08/01/2020
    Requires: Clock.h/c
    ***************************************************
    Sends IR codes on P2.5 using a 38kHz carrier signal.

    Proof-of-concept for my IR-RX/TX BoosterPack for the
    TI Summer Project 2020.
    ***************************************************
*/

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


#include "msp.h"
#include "Clock.h"
#include <stdint.h>
#include <time.h>



// EFFECT: initializes the PWM to send IR codes on P2.5
//         duty can be 0-1 (0-100%)
//         period is in seconds
void PWM_Init(float period, float duty)
{
    // Setup P2.5
    P2->DIR |= 0x20; // Set bit 5
    P2->SEL0 |= 0x20; // Set bit 5
    P2->SEL1 &= ~0x20; // Clear bit 5

    // Setup timer
    TIMER_A0->CCTL[0] = 0x0080; // Set bit 7 for toggle mode

    // CCR0 is how high the timer counts
    // period = 2*CCRO*8/12MHz
    // CCRO = 12MHz*period/16
    float CCR0 = period*12000000/2;
    TIMER_A0->CCR[0] = CCR0;

    // CCR1 is when the timer toggles the output
    // the timer will toggle when it equals CCR1 on the way up
    // and again on the way back down
    // duty = CCR1/CCR0
    // CCR1 = CCRO*duty
    float CCR2 = CCR0*duty;
    TIMER_A0->CCR[2] = CCR2;

    TIMER_A0->EX0 = 0x0000; // divide the input clock by 1

    TIMER_A0->CCTL[2] = 0x0040; // CCR2 toggle/reset

    // Bit | Name   | Value | Effect
    // -------------------------------------------------
    // 9-8 | TASSEL | 10b   | SMCLK as clock source (CS)
    // 7-6 | ID     | 00b   | divide CS by 1
    // 5-4 | MC     | 11b   | Up/down mode - counts up to TA0CCR0 and down to 0
    // Here we only set the TASSEL bits so the PWM is not enabled yet
    // the MC bits will be set by enablePWM()
    TIMER_A0->CTL = 0x0200;
}

// EFFECT: sets the MC bits of TimerA0 to enable the PWM pulse
void enablePWM(void)
{
    TIMER_A0->CTL |= 0x0030;
}

// EFFECT: clears the MC bits of TimerA0 to disable the PWM pulse
void disablePWM(void)
{
    TIMER_A0->CTL &= ~0x0030;
}

// EFFECT: initializes SysTick to time pulses
void SysTick_Init(void)
{
    // Load in max starting value
    SysTick->LOAD = 0x00FFFFFF;
    // Enable and set clock source to the bus (48MHz)
    SysTick->CTRL = 0x00000005;
}

// EFFECT: calls enablePWM() for usec microseconds and then calls disablePWM()
void mark(unsigned int usec)
{
    int clockCycles = usec * 48;
    SysTick->LOAD = clockCycles-1;
    SysTick->VAL = 0;
    while ((SysTick->CTRL&0x00010000) == 0)
    {
        enablePWM();
    }
    disablePWM();
}

// EFFECT: does nothing (waits) for usec
void space(unsigned int usec)
{
    int clockCycles = usec * 48;
    SysTick->LOAD = clockCycles-1;
    SysTick->VAL = 0;
    while ((SysTick->CTRL&0x00010000) == 0)
    {}
}

// EFFECT: sends the array of pulses cmd[] with pauseTimes[] between each pulse
void sendCommand(unsigned int cmd[], unsigned int pause[], unsigned int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        mark(cmd[i]);
        space(pause[i]);
    }
}

// EFFECT: calls all initialization functions
void _initPWM(void)
{
    Clock_Init48MHz();
    SysTick_Init();
    PWM_Init(0.000026315789473684, 0.5); // init pwm on pin P2.5 with 38kHz and 50% duty cycle
}



unsigned int up_matrix[5][MAX_PULSE] =      {{1950,250,250,400,250,250,250,300,300,250,300,200,250,250,250,250,300,250,250,200,400,350,200,250,250,250,400,300,350,250,250,400,350,250},
                                             {1950,250,250,350,300,400,300,200,250,300,250,250,250,250,300,250,250,250,300,400,200,250,300,350,350,250,350,300,300,350,400,400,350,250},
                                             {1950,300,350,350,250,400,250,250,400,300,350,350,300,250,250,300,300,250,300,350,250,250,250,200,250,400,300,350,250,300,350,250,300,250},
                                             {1950,250,300,350,250,250,300,200,400,250,350,350,250,250,200,250,250,250,250,300,350,250,250,300,300,300,350,350,400,350,300,300,300,250},
                                             {1950,300,350,250,250,350,300,250,400,250,400,350,250,250,300,300,250,250,250,250,250,250,250,300,300,250,250,300,400,350,350,350,300,250}};

unsigned int up_matrix_p[5][MAX_PULSE] =    {{1950,350,200,700,350,750,700,250,750,300,350,750,750,700,750,700,700,750,350,250,200,750,750,750,750,200,250,300,250,400,200,250,750,1000},
                                             {1950,400,200,700,200,700,700,400,700,300,350,750,750,650,700,750,750,700,200,700,400,700,200,250,750,200,300,300,250,200,200,200,750,1000},
                                             {1950,200,250,700,200,750,700,200,750,200,250,700,700,700,750,700,650,750,200,700,350,750,750,750,200,300,200,400,300,200,350,300,700,1000},
                                             {1950,350,200,750,350,700,700,250,750,200,250,750,750,700,750,750,750,700,750,200,750,700,300,700,700,200,250,200,200,350,250,300,750,1000},
                                             {1950,200,350,750,200,750,700,200,700,200,250,750,700,700,700,700,750,700,750,750,700,750,250,750,700,300,350,200,200,250,200,350,700,1000}};

unsigned int fw_matrix[5][MAX_PULSE] =      {{1950,250,400,250,350,400,400,250,400,400,350,400,400,400,350,400,450,250,250,400,400,250,400,250,200,400,350,300,350,400,400,300,350,400},
                                             {1950,250,350,250,350,400,350,300,350,400,400,400,400,350,450,400,400,350,300,350,350,300,300,300,300,300,350,250,400,400,400,250,350,400},
                                             {1900,200,450,250,350,400,400,300,350,400,400,350,350,400,400,400,400,250,250,400,250,300,350,250,400,450,300,300,350,400,450,250,400,400},
                                             {1950,250,400,250,400,350,400,300,400,350,400,400,400,400,350,450,350,250,300,300,350,350,350,350,250,250,350,200,450,350,450,250,400,400},
                                             {1950,250,400,250,400,350,450,300,350,400,350,400,400,400,400,400,400,250,250,350,300,300,250,400,250,250,300,300,350,400,400,250,450,250}};

unsigned int fw_matrix_p[5][MAX_PULSE] =    {{1950,200,750,200,200,200,750,200,200,200,200,200,200,200,200,200,700,700,200,200,750,200,700,750,250,200,750,200,200,200,700,200,200,1000},
                                             {2000,200,800,200,200,200,750,200,200,200,200,200,200,200,150,200,700,650,200,200,750,700,700,650,700,200,700,200,200,200,750,200,200,1000},
                                             {2000,200,750,200,200,200,750,150,200,200,200,250,200,200,200,200,750,700,200,750,700,200,700,200,200,200,750,200,200,200,650,250,200,1000},
                                             {1950,200,750,200,200,200,700,200,200,200,200,200,200,200,200,200,750,700,700,200,700,200,200,750,750,200,750,200,200,200,700,200,200,1000},
                                             {1950,200,750,200,200,200,700,200,200,200,200,200,200,200,200,200,700,750,700,650,650,700,200,750,750,250,750,200,200,200,700,150,750,1000}};

unsigned int bk_matrix[5][MAX_PULSE] =      {{1950,250,400,400,300,250,250,250,350,300,350,300,250,250,300,350,300,300,300,350,400,250,400,450,350,250,350,300,350,350,450,300,350,300},
                                             {1950,250,400,400,300,300,250,300,300,250,400,200,300,300,250,400,300,300,250,400,300,350,400,400,350,250,400,250,350,400,400,300,400,350},
                                             {1950,250,400,400,250,350,250,300,350,300,350,250,350,300,300,350,250,300,250,350,250,250,400,350,400,400,350,250,350,400,350,400,300,300},
                                             {1900,250,400,400,250,250,300,250,300,300,350,350,250,300,300,350,250,350,250,250,350,250,350,450,350,350,450,250,400,350,450,250,400,400},
                                             {1900,250,400,400,250,300,300,300,300,300,350,300,250,350,250,350,250,300,250,250,250,300,200,300,300,200,450,300,400,350,400,250,400,400}};

unsigned int bk_matrix_p[5][MAX_PULSE] =    {{1950,200,200,750,650,750,700,750,650,200,750,650,750,700,200,750,700,650,200,200,700,200,200,200,750,200,750,200,200,200,700,200,750,1000},
                                             {1950,200,200,750,700,650,750,700,650,200,750,800,650,700,200,700,700,700,200,700,200,200,200,200,750,200,750,200,200,200,700,200,200,1000},
                                             {1950,200,200,700,750,650,750,650,650,200,750,700,650,650,200,750,750,700,200,750,750,200,200,200,200,200,800,200,200,200,200,750,700,1000},
                                             {2000,200,200,750,700,750,650,750,700,200,700,650,700,700,200,750,700,700,700,200,800,200,200,200,200,200,700,200,200,200,700,200,200,1000},
                                             {2000,200,200,700,750,700,700,650,700,200,700,700,700,700,200,750,700,750,700,750,700,700,800,650,700,200,700,200,200,200,700,200,200,1000}};

unsigned int left_matrix[5][MAX_PULSE] =    {{1950,250,400,250,250,300,350,350,250,350,400,400,300,250,250,250,250,250,300,350,400,350,250,250,250,250,400,250,350,400,400,250,400,300},
                                             {1950,250,450,200,250,350,350,400,300,350,350,400,300,300,300,300,250,300,250,400,400,250,250,400,400,250,400,250,400,350,400,250,400,250},
                                             {1950,250,400,250,300,300,350,350,300,350,350,400,250,250,250,250,250,250,250,400,250,250,400,400,350,300,350,250,400,350,450,250,400,250},
                                             {1950,250,400,300,250,300,300,400,250,400,350,400,250,250,250,250,250,250,250,250,400,250,400,400,400,350,350,250,400,350,350,300,350,250},
                                             {1950,250,400,300,300,350,300,400,250,400,400,400,250,250,300,250,250,300,250,300,300,250,350,350,300,350,350,250,350,400,400,250,400,250}};

unsigned int left_matrix_p[5][MAX_PULSE] =  {{1950,200,700,750,750,200,200,800,200,200,200,700,700,750,700,750,700,750,200,200,200,800,700,750,700,200,750,200,200,200,750,200,700,1000},
                                             {1950,200,700,750,750,150,200,750,200,200,200,700,700,700,650,700,750,650,200,200,700,750,200,200,750,200,700,200,200,200,750,200,700,1000},
                                             {1950,200,750,700,700,200,200,750,200,200,250,700,750,750,700,750,750,700,200,750,700,200,200,200,750,200,750,200,200,200,650,200,800,1000},
                                             {1950,200,750,650,750,250,200,750,200,200,200,750,750,700,750,750,700,750,700,200,700,200,200,200,700,200,700,250,200,200,750,200,700,1000},
                                             {1950,200,750,700,650,200,200,700,200,200,200,750,750,700,700,750,700,650,750,700,700,700,150,750,650,200,700,250,200,200,700,200,750,1000}};

unsigned int right_matrix[5][MAX_PULSE] =   {{1900,300,350,400,400,400,250,250,400,250,400,400,250,200,300,250,300,250,250,400,400,350,300,200,250,300,350,250,400,450,350,250,250,350},
                                             {1950,250,400,400,400,350,250,250,400,350,400,400,250,250,250,300,250,250,250,400,400,250,250,250,450,250,350,250,400,350,400,300,400,300},
                                             {1950,250,400,400,400,350,200,250,400,450,400,400,250,250,250,250,250,250,250,400,250,300,400,350,250,350,450,250,400,350,450,200,450,250},
                                             {1950,250,400,350,400,400,250,300,400,350,400,400,250,250,250,250,200,250,250,250,400,350,250,250,300,200,400,300,400,350,400,250,350,250},
                                             {1950,250,400,400,350,400,250,250,350,400,400,400,250,250,250,250,250,250,300,250,300,300,250,400,250,300,400,350,350,350,400,250,400,300}};

unsigned int right_matrix_p[5][MAX_PULSE] = {{2000,200,200,200,200,700,700,200,750,200,200,700,750,800,650,750,700,700,200,200,200,750,700,800,700,200,700,200,200,200,750,750,200,1000},
                                             {2000,200,200,200,200,750,750,200,200,200,200,750,700,750,700,700,750,700,200,200,750,750,650,200,750,200,750,200,200,200,700,200,700,1000},
                                             {2000,200,200,200,200,750,750,200,200,150,200,750,700,750,700,750,750,700,200,750,700,200,200,750,200,200,700,200,200,200,700,200,700,1000},
                                             {2000,200,200,200,200,750,700,200,200,200,200,750,700,750,700,750,800,700,750,200,200,750,750,700,700,250,700,200,200,200,750,200,750,1000},
                                             {2000,200,200,200,200,750,750,200,200,200,200,750,700,750,700,750,750,700,700,750,650,700,200,700,750,150,700,200,200,200,700,200,750,1000}};




/* Statics */
#define TIMER_PERIOD    0x0FA0   //4000 to hex
#define TIMER_PERIOD2    0x1770   //6000 to hex


/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfigTimerA3 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

const Timer_A_UpModeConfig upConfigTimerA2 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD2,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};



void _PCM_Flash_Init()
{
    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
}


void _ledInit()
{
    /* Configuring P1.0 as output */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}


void _timersInit()
{
        /* Configuring Timer_A1 for Up Mode */
        Timer_A_configureUpMode(TIMER_A3_BASE, &upConfigTimerA3);
        Timer_A_configureUpMode(TIMER_A2_BASE, &upConfigTimerA2);

        /* Enabling interrupts and starting the timer */
        //Interrupt_enableSleepOnIsrExit();
        Interrupt_enableInterrupt(INT_TA3_0);
        Interrupt_enableInterrupt(INT_TA2_0);
}


//Buttons on TI J4.32 and J4.33
void _buttonsInit()
{
    //Button S1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    //Button S2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);


    //enable interrupts for Port3 and Port5
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT3);
}



//initialize Analog to Digital Converter for Joystick and enables its interrupts
void _adcInit()
{
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
         * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);

    /* Setting up the sample timer to manually step through the sequence*/
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
}


//initialize Liquid Crystal Display
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
=======
#include "CommandMatrices.h"
#include "SendCodes_main.h"
#include "HardwareInit.h"

>>>>>>> nuovobuggy


void _hwInit()
{
<<<<<<< HEAD
=======
    //initialize hardware for PWM and IR emitter
    _initPWM();

>>>>>>> nuovobuggy
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

<<<<<<< HEAD
    //initialize hardware for PWM and IR emitter
    _initPWM();

=======
>>>>>>> nuovobuggy
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


<<<<<<< HEAD
    ADC14_toggleConversionTrigger();

=======
>>>>>>> nuovobuggy
    // send a command
    while(1)
    {
    }

}

<<<<<<< HEAD

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
=======
//global variables for current speed state and information about next command to be sent
int curr_val = 0;
int forw_backw = 0;
int right_left = 0;


//finds the right command to send and calls sendCommand to send it
void findCommand()
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


//HARDWARE INTERRUPTS



//The two Timers alternate in triggering interrupts; this is necessary as the real remote control emits IR signals with different time intervals
//Note I'm not calling findCommand() inside the TIMER Interrupts (ex. after ADC14_toggleConversionTrigger()); as this function is called AT THE END of the timer interrupt
//This would mean not getting a responsive device.

//Timer A2 Capture/Compare Register 0 Interrupt handler
void TA2_0_IRQHandler(void)
{
        //trigger ADC conversion of Joystick position to get next command to send
        ADC14_toggleConversionTrigger();
>>>>>>> nuovobuggy


        Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE,
                TIMER_A_CAPTURECOMPARE_REGISTER_0);

<<<<<<< HEAD
        Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
        Timer_A_stopTimer(TIMER_A2_BASE);


        ADC14_toggleConversionTrigger();
}



/* ADC results buffer */
static uint16_t resultsBuffer[2];

=======
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
>>>>>>> nuovobuggy

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
<<<<<<< HEAD
=======
    /* ADC results buffer */
>>>>>>> nuovobuggy
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

<<<<<<< HEAD
=======

>>>>>>> nuovobuggy
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

<<<<<<< HEAD
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
=======

        //manipulate Joystick X position and decide accordingly whether to turn left, right or stay still
        right_left = 0; //reset right-left state (stop turning)

        if(resultsBuffer[0] > 14000)
        {
            right_left = 1;  //right
        }
        if(resultsBuffer[0] < 2000)
        {
            right_left = -1;  //left
>>>>>>> nuovobuggy
        }


        sprintf(string, "Y: %5d", resultsBuffer[1]);
        Graphics_drawStringCentered(&g_sContext,
                                        (int8_t *)string,
                                        8,
                                        64,
                                        70,
                                        OPAQUE_TEXT);

<<<<<<< HEAD
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
=======
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
>>>>>>> nuovobuggy
    }
}



//Button S2 on BoosterPack Module
void PORT3_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
<<<<<<< HEAD
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN5)){
        /* Toggling the output on the LED */


=======

    /* check if we received P3.5 interrupt */
    if((status & GPIO_PIN5)){
>>>>>>> nuovobuggy
        if(curr_val == 0)
        {
            //won't decrement as velocity matrix index can't go under 0 (vector[-1] doesn't exist)
        }
<<<<<<< HEAD
=======
        else
>>>>>>> nuovobuggy
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
<<<<<<< HEAD
    /* check if we received P1.1 or P1.4 interrupt */
    if((status & GPIO_PIN1)){
        /* Toggling the output on the LED */

=======

    /* check if we received P5.1 interrupt */
    if((status & GPIO_PIN1)){
>>>>>>> nuovobuggy
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
