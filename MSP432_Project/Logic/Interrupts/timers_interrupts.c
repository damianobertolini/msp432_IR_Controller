/*
 * timers_interrupts.c
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#include "interrupts.h"

// for testing
#include "Logic/test.h"


//The two Timers alternate in triggering interrupts; this is necessary as the real remote control emits IR signals with different time intervals
//Note I'm not calling findCommand() inside the TIMER Interrupts (ex. after ADC14_toggleConversionTrigger()); as this last function is called AT THE END of the timer interrupt
//(This would mean not getting a responsive device)


//Timer A2 Capture/Compare Register 0 Interrupt handler
void TA2_0_IRQHandler(void)
{
    //stopping timer A2 and starting timer A3 -> sending signals with different time intervals
    Timer_A_stopTimer(TIMER_A2_BASE);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);


    if(TEST)
    {
        test_timers(2);
        return;
    }

    // if the Bluetooth mode was selected it calls the BLE manager, else calls the ADC Conversion for Joystick and accelerometer
    if(mode_selected && currentSelection == BLUETOOTH)
    {
        ble_command_manager();
    }
    else
    {
        //trigger ADC conversion of Joystick position to get next command to send
        ADC14_toggleConversionTrigger();
    }
}

//Timer A3 Capture/Compare Register 0 Interrupt handler
void TA3_0_IRQHandler(void)
{
    //stopping timer A3 and starting timer A2 -> sending signals with different time intervals
    Timer_A_stopTimer(TIMER_A3_BASE);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);


    if(TEST)
    {
        test_timers(1);
        return;
    }

    // if the Bluetooth mode was selected it calls the BLE manager, else calls the ADC Conversion for Joystick and accelerometer
    if(mode_selected && currentSelection == BLUETOOTH)
    {
        ble_command_manager();
    }
    else
    {
        //trigger ADC conversion of Joystick position to get next command to send
        ADC14_toggleConversionTrigger();
    }
}

// variables used for timers testing
int n_loops_timer_id = 0;
int done_timer_testing = 0;
volatile int timer_test_ongoing = 0;
int prev_timer = 0;


// tests if timers are actually alternating
void test_timers(int timer_id)
{
    if(n_loops_timer_id == 10)
    {
        if(!done_timer_testing)
        {
            printf("Timer Test OK\n");  //they are alternating

            printf("Finished testing timers\n\n");
            done_timer_testing = 1;
            timer_test_ongoing = 0;
        }

        return; //stop testing timers
    }

    if(!prev_timer)
    {
        prev_timer = timer_id;
    }
    else
    {
        if(prev_timer == timer_id)
        {
            printf("Timer Test FAILED\n");  //they aren't alternating

            printf("Finished testing timers\n\n");
            done_timer_testing = 1;
            timer_test_ongoing = 0;

            return;
        }
        else
        {
            prev_timer = timer_id;
        }
    }

    n_loops_timer_id++;

    printf("Timer ID: %d\n", timer_id);
}

// function called by main which disables other interrupts and start only the timer it wants to start counting
void test_timers_main()
{
    //otherwise the following printf operation would take too much time and the first timers interrupts would block this function and all the following tests in main
    Timer_A_stopTimer(TIMER_A2_BASE);
    Timer_A_stopTimer(TIMER_A3_BASE);


    printf("\n Testing timers, checking they alternate between each other\n");

    Interrupt_enableMaster();
    ADC14_disableConversion();

    timer_test_ongoing = 1;

    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    while(timer_test_ongoing)
    {
        //wait for interrupts to stop timer test and exit this loop
    }

    ADC14_enableConversion();
}

