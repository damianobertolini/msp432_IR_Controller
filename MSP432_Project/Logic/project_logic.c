/*
 * project_logic.c
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#include <stdio.h>

// Infrared
#include "Hardware/Infrared/command_matrices.h"
#include "Hardware/Infrared/send_codes.h"

// Pure hardware header file
#include "Hardware/hardware_init.h"

// Graphics
#include "Hardware/Lcd/Crystalfontz128x128_ST7735.h"
#include "Hardware/Graphics/graphics_context.h"   //imported twice (present also in hardware_init.h) just to avoid problems in case of future changes in hardware_init.h
#include "Hardware/Graphics/direction_graphics.h"
#include "Hardware/Graphics/images.h"
#include "Hardware/Graphics/img_vars.h"

// Bluetooth
#include "Hardware/Bluetooth/Controller/UART_IO.h"

// global variables also used in main
int curr_val = 0;
int forw_backw = 0;
int right_left = 0;

Selection_t currentSelection = JOYSTICK;


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
}




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

void land()
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
        land();
    }

    switch(Buffer[0])
    {
    case 'f':
        forw_backw = 1;
        Graphics_drawImage(&g_sContext, &FORWARD_RED, 46, 8);
        Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
        Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
        Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
        break;
    case 'b':
        forw_backw = -1;
        Graphics_drawImage(&g_sContext, &FORWARD_WHITE, 46, 8);
        Graphics_drawImage(&g_sContext, &BACKWARD_RED, 46, 76);
        Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
        Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
        break;
    case 'r':
        right_left = 1;
        Graphics_drawImage(&g_sContext, &FORWARD_WHITE, 46, 8);
        Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
        Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
        Graphics_drawImage(&g_sContext, &RIGHT_RED, 76, 46);
        break;
    case 'l':
        right_left = -1;
        Graphics_drawImage(&g_sContext, &FORWARD_WHITE, 46, 8);
        Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
        Graphics_drawImage(&g_sContext, &LEFT_RED, 8, 46);
        Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
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
        land();
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
        currentSelection = (Selection_t) ((3 + --currentSelection) % 3);  // Selection_t casting in order to avoid "enumerated type mixed with another type" warning
    } else if(y<7000){
        currentSelection = (Selection_t) (++currentSelection % 3);        // Selection_t casting in order to avoid "enumerated type mixed with another type" warning
    }


    if(currentSelection == JOYSTICK){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLUE, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLACK, 17, 75);
        Graphics_drawImage(&g_sContext, &BLUETOOTH_BLACK, 17, 92);
        Graphics_drawImage(&g_sContext, &BLUETOOTH_LOGO_BLACK, 101, 93);
    }

    if(currentSelection == ACCELEROMETER){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLACK, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLUE, 17, 75);
        Graphics_drawImage(&g_sContext, &BLUETOOTH_BLACK, 17, 92);
        Graphics_drawImage(&g_sContext, &BLUETOOTH_LOGO_BLACK, 101, 93);
    }

    if(currentSelection == BLUETOOTH){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLACK, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLACK, 17, 75);
        Graphics_drawImage(&g_sContext, &BLUETOOTH_BLUE, 17, 92);
        Graphics_drawImage(&g_sContext, &BLUETOOTH_LOGO_BLUE, 101, 93);
    }
}

