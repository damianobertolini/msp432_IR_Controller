/*
 * timers_interrupts.c
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#include "interrupts.h"


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



