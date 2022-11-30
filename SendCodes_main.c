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

#include "msp.h"
#include "Clock.h"
#include <stdint.h>
#include <time.h>



// Commands start with a 5000 usec pulse to increase reliability
// 500 usec pulse denotes a 0
// 2000 usec pulse denotes a 1


// There is a 1000 usec pause between each pulse


int i;


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
    for (i = 0; i < length; i++)
    {
        mark(cmd[i]);
        space(pause[i]);
    }
}

// EFFECT: calls all initialization functions
void initAll(void)
{
    Clock_Init48MHz();
    SysTick_Init();
    PWM_Init(0.000026315789473684, 0.5); // init pwm on pin P2.5 with 38kHz and 50% duty cycle
}

