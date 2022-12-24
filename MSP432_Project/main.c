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
#include "direction_graphics.h"
#include <images.h>
#include <img_vars.h>
#include <BluetoothDriver/Hardware/CS_Driver.h>
#include <BluetoothDriver/Devices/MSPIO.h>


void _hwInit()
{
    //Initializes all unused ports in order to address power consumption, this function is not mandatory and should just remove the warning (see the implementation for further info on why the warning is still present):
    //"Detected uninitialized Port 1 in this project. Recommend initializing all unused ports to eliminate wasted current consumption on unused pins."
    _lowPowerInit();

    //initialize hardware for PWM and IR emitter
    _initPWM();

    //sets core voltage level and number of wait states used by flash controller for read operation
    _PCM_Flash_Init();

    //initialize LED on Pin 1.0
    _ledInit();

    //initialize TimerA2 and TimerA3 into upMode
    _timersInit();

    //initialize buttons S1 and S2 (pins J4.32 and J4.33) on BoosterPack Module and Joystick button (Port4 PIN 1)
    _buttonsInit();
    
    //initialize ADC for Joystick
    _joystickInit();

    //initialize ADC for accelerometer
    _accelSensorInit();

    //initialize general ADC
    _adcInit();

    //initialize Bluetooth Module with UART
    _bluetoothInit();

    //initialize LCD
    _graphicsInit();

    //enable master interrupt
    Interrupt_enableMaster();

    //start first timer (TIMER_A3)
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}


void main(void)
{
    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    _hwInit();

    MSPrintf(EUSCI_A2_BASE, "Enter data to be sent to HC-05");

    // send a command
    while(1)
    {
        PCM_gotoLPM0();
    }
}

//global variables for current speed state and information about next command to be sent
int curr_val = 0;
int forw_backw = 0;
int right_left = 0;
int y_value;

Selection_t currentSelection = JOYSTICK;
int mode_selected = 0;




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

int n_plane = 0;
int planing = 0;

void plane()
{
    if(curr_val != 0)
    {
        if(n_plane == 4)
        {
            curr_val--;
            n_plane = 0;
        }
        else
        {
            n_plane++;
        }

        findCommand();

        customDelay(500000);
    }
    else
    {
        planing = 0;
    }
}


void ble_command_manager()
{
    //this custom MSPgets won't block other functions waiting for a command to be sent via Bluetooth and will just check if the UART Buffer is not empty
    customMSPgets(EUSCI_A2_BASE, Buffer, BLUETOOTH_BUFFER_SIZE);

    if(planing)
    {
        plane();
    }

    switch(Buffer[0])
    {
    case 'f':
        forw_backw = 1;
        break;
    case 'b':
        forw_backw = -1;
        break;
    case 'r':
        right_left = 1;
        break;
    case 'l':
        right_left = -1;
        break;
    case 'u':
        if(curr_val != 4)
        {
            //increase speed
            curr_val++;
        }
        break;
    case 'd':
        if(curr_val != 0)
        {
            //decrease speed
            curr_val--;
        }
        break;
    case 'p':
        planing = 1;
        plane();
        break;

    //no need of default (could be useful for debugging purposes)
    }

    //empty buffer in order not to read the same character at every iteration
    Buffer[0]= 0;

    //decide which command to send and emit IR signals accordingly
    findCommand();

    //toggle LED state in order to notify the user that a command has been sent (mostly useful for debugging purposes)
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void drawSelection(int y){

    if(y>9800){
        currentSelection = JOYSTICK;
    } else if(y<7000){
        currentSelection = ACCELEROMETER;
    }

    if(currentSelection == JOYSTICK){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLUE, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLACK, 17, 75);
    }

    if(currentSelection == ACCELEROMETER){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLACK, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLUE, 17, 75);
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


//HARDWARE INTERRUPTS


//The two Timers alternate in triggering interrupts; this is necessary as the real remote control emits IR signals with different time intervals
//Note I'm not calling findCommand() inside the TIMER Interrupts (ex. after ADC14_toggleConversionTrigger()); as this function is called AT THE END of the timer interrupt
//This would mean not getting a responsive device.


int global = 0;

//Timer A2 Capture/Compare Register 0 Interrupt handler
void TA2_0_IRQHandler(void)
{
    //stopping timer A2 and starting timer A3 -> sending signals with different time intervals
    Timer_A_stopTimer(TIMER_A2_BASE);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);

    global = 1;

    currentSelection = BLUETOOTH;

    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

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

    global = 1;


    Timer_A_clearCaptureCompareInterrupt(TIMER_A3_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

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
void TA1_0_IRQHandler(void)
{
    //trigger ADC conversion of Joystick and accelerometer position
    ADC14_toggleConversionTrigger();


    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}



/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1 or ADC_MEM2. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    
    /* ADC results buffer that can store both the Joystick results and accelerometer ones */
    uint16_t resultsBuffer[3];

    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    right_left = 0; //reset right-left state (stop turning)
    forw_backw = 0;  //reset forw-backw state (stop moving)


    //still in menu mode
    if(!mode_selected)
    {
        //joystick
        if(status & ADC_INT1)
        {
            /* Store ADC14 conversion results */
            resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

            //switch modality in menu
            drawSelection((int) resultsBuffer[1]);
        }

        return;
    }


    int x_value;                            // integer that contains the x value of the joystick
    int y_value;                            // integer that contains the y value of the joystick


    /* ADC_MEM1 or ADC_MEM2 conversion completed */
    //joystick data ready
    if((status & ADC_INT1) && currentSelection == JOYSTICK)
    {
        /* Store ADC14 conversion results */
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        //draw Direction power
        x_value = (int) resultsBuffer[0];
        y_value = (int) resultsBuffer[1];

        drawDirections(x_value, y_value, currentSelection);

        if(!global)
        {
            return;
        }


        //manipulate Joystick X position and decide accordingly whether to turn left, right or stay still


        if(resultsBuffer[0] > 14000)
        {
            right_left = 1;  //right
        }
        if(resultsBuffer[0] < 2000)
        {
            right_left = -1;  //left
        }


        //manipulate Joystick Y position and decide accordingly whether to move forward, backward or stay still

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

        global = 0;
    }
    else
    {
        //accelerometer data ready
        if ((status & ADC_INT4) && currentSelection == ACCELEROMETER)
        {

            /* Store ADC14 conversion results */
            resultsBuffer[0] = ADC14_getResult(ADC_MEM2);
            resultsBuffer[1] = ADC14_getResult(ADC_MEM3);
            resultsBuffer[2] = ADC14_getResult(ADC_MEM4);


            //draw Direction power
            x_value = (int) resultsBuffer[0];
            y_value = (int) resultsBuffer[1];
            drawDirections(x_value, y_value, currentSelection);


            if(!global)
            {
                return;
            }

            if(resultsBuffer[0] < 6500)
            {
                right_left = -1;
            }
            if(resultsBuffer[0] > 9800)
            {
                right_left = 1;
            }

            if(resultsBuffer[1] < 7000)
            {
                forw_backw = -1;
            }
            if(resultsBuffer[1] > 9600)
            {
                forw_backw = 1;
            }

            //decide which command to send and emit IR signals accordingly
            findCommand();

            //toggle LED state in order to notify the user that a command has been sent (mostly useful for debugging purposes)
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

            //printf("X-%d, Y-%d, Z-%d\n", resultsBuffer[0],resultsBuffer[1],resultsBuffer[2]);

            global = 0;
        }
    }
}


//Joystick button on BoosterPack Module
void PORT4_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    /* check if we received P3.5 interrupt */
    if((status & GPIO_PIN1)){
        //user has chosen modality
        if(!mode_selected)
        {
            //not modifiable anymore
            mode_selected = 1;

            Graphics_drawImage(&g_sContext, &DIRECTIONS, 0, 0);
        }
    }
}


//Button S2 on BoosterPack Module
//note that strangely PORT3 has an interrupt at the start of execution process, seemingly not related to a particular PIN
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
        else
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
