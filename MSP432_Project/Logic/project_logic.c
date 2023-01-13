/*
 * project_logic.c
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */
#include "project_logic.h"

// Infrared
#include "Hardware/Infrared/command_matrices.h"
#include "Hardware/Infrared/send_codes.h"

// Pure hardware header file
#include "Hardware/hardware_init.h"

// Graphics
#include "Hardware/Lcd/Crystalfontz128x128_ST7735.h"

#include "Hardware/Graphics/graphics_context.h"   //imported twice (present also in hardware_init.h) just to avoid problems in case of future changes in hardware_init.h
#include "Hardware/Graphics/images_definitions.h"
#include "Hardware/Graphics/direction_graphics.h"

// Bluetooth
#include "Hardware/Bluetooth/Controller/UART_IO.h"

// for testing
#include "Logic/test.h"

// defining global variables also used in main
uint8_t curr_val = 0;
int8_t forw_backw = 0;
int8_t right_left = 0;

Selection_t currentSelection = JOYSTICK;    // This way the Joystick will be shown as selected at the startup of the menu

//initializes all hardware
void _hwInit()
{
    //Initializes all unused ports in order to address power consumption, this function is not mandatory and should just remove the warning (see the implementation for further info on why the warning is still present):
    //"Detected uninitialized Port 1 in this project. Recommend initializing all unused ports to eliminate wasted current consumption on unused pins."
    _lowPowerInit();

    //initialize hardware for PWM and IR emitter
    _initPWM();

    //sets core voltage level and number of wait states used by flash controller for read operation
    _PCM_Flash_WDT_Init();

    //initialize LED on Pin 1.0
    _ledInit();

    //initialize TimerA2 and TimerA3 into upMode
    _timersInit();

    //initialize buttons S1 and S2 (pins J4.32 and J4.33) on BoosterPack Module and Joystick button (Port4 PIN 1)
    _buttonsInit();

    // it is possible to initialize Joystick or accelerometer only if they have been selected by the user in the menu, but this configuration could allow to use both at the same time

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
}

// start the first helicopter image at the startup
void start_graphics()
{
    // outputs helicopter image
    startImageHelicopter();
}

// draws the menu
void start_menu()
{
    drawMenu();
}

// activates all peripherals interrupts and start the first of the two timers which manage the IR emission
void activate_peripherals()
{
    // enable master interrupt
    Interrupt_enableMaster();

    // start first timer (TIMER_A3)
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
}


// finds the right command to send and calls sendCommand() to send it
void findCommand()
{
    //create in-function local copy of global variables in order to have the possibility to erase their value
    int8_t curr_forw_backw = forw_backw;
    int8_t curr_right_left = right_left;


    //erase value (user will have to trigger again the device (Joystick, accelerometer, ...) in order to put them back to 1
    forw_backw = 0;
    right_left = 0;

    //toggle LED state in order to notify the user that a command has been sent (mostly useful for debugging purposes)
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    //forward or backward
    if(curr_forw_backw == 1)
    {
        #ifdef TEST
        test_command = 1;
        return;
        #endif

        sendCommand(fw_matrix[curr_val], fw_matrix_p[curr_val], sizeof(fw_matrix[curr_val]) / sizeof(fw_matrix[curr_val][0]));

        return;
    }
    if(curr_forw_backw == -1)
    {
        #ifdef TEST
        test_command = 2;
        return;
        #endif

        sendCommand(bk_matrix[curr_val], bk_matrix_p[curr_val], sizeof(bk_matrix[curr_val]) / sizeof(bk_matrix[curr_val][0]));
        return;
    }

    //right or left
    if(curr_right_left == 1)
    {
        #ifdef TEST
        test_command = 3;
        return;
        #endif

        sendCommand(right_matrix[curr_val], right_matrix_p[curr_val], sizeof(right_matrix[curr_val]) / sizeof(right_matrix[curr_val][0]));
        return;
    }
    if(curr_right_left == -1)
    {
        #ifdef TEST
        test_command = 4;
        return;
        #endif

        sendCommand(left_matrix[curr_val], left_matrix_p[curr_val], sizeof(left_matrix[curr_val]) / sizeof(left_matrix[curr_val][0]));
        return;
    }

    //default maintain current propeller speed
        #ifdef TEST
        test_command = 5;
        return;
        #endif

    sendCommand(up_matrix[curr_val], up_matrix_p[curr_val], sizeof(up_matrix[curr_val]) / sizeof(up_matrix[curr_val][0]));
}


uint8_t n_plane = 0;
uint8_t landing = 0;

// local function called when using Bluetooth and the user presses on "Plane", it slowly decrements the propellers power in order to make the helicopter land
void land()
{
    #ifdef TEST
    printf("curr_val = %d, n_plane = %d\n", curr_val, n_plane);
    #endif

    // if the current velocity isn't 0, it counts 4 timers (A2 and A3) interrupts then decrements it until its value gets to 0
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

        // It still needs to send signals even if n_plane isn't 4, otherwise the helicopter would stop for a moment
        findCommand();
    }
    else
    {
        // after finishing the landing, landing variable is set to 0 so that land() will not be called anymore
        landing = 0;
        return;
    }

    #ifdef TEST
    //recursive call
    land();
    #endif

}


// this functions implements the logical control of the Bluetooth module, interpreting received values and taking the appropriate actions
void ble_command_manager()
{
    //this custom MSPgets won't block other functions waiting for a command to be sent via Bluetooth and will just check if the UART Buffer is not empty
    customMSPgets(EUSCI_A2_BASE, Buffer, BLUETOOTH_BUFFER_SIZE);

    // don't want to listen to other commands if the helicopter is in the landing phase
    if(landing)
    {
        land();
    }

    // check value read from the Bluetooth UART Buffer
    switch(Buffer[0])
    {
    case 'f':
        forw_backw = 1;
        drawDirectionForward();
        break;
    case 'b':
        forw_backw = -1;
        drawDirectionBackward();
        break;
    case 'r':
        right_left = 1;
        drawDirectionRight();
        break;
    case 'l':
        right_left = -1;
        drawDirectionLeft();
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
        landing = 1;
        land();
        break;

    //no need of default (could be useful for debugging purposes)
    }

    //empty buffer in order not to read the same character at every iteration
    Buffer[0]= 0;

    //decide which command to send and emit IR signals accordingly
    findCommand();
}


// move between the three menu options and redraws the option images based on the current selection
// the upper and lower bound for the y value received are set to 9800 and 7000 in order to ignore little and unwanted movements
void drawSelection(uint64_t y){

    if(y>9800){
        currentSelection = (Selection_t) ((3 + --currentSelection) % 3);  // Selection_t casting in order to avoid "enumerated type mixed with another type" warning
    } else if(y<7000){
        currentSelection = (Selection_t) (++currentSelection % 3);        // Selection_t casting in order to avoid "enumerated type mixed with another type" warning
    }

    if(currentSelection == JOYSTICK){
        drawSelectionJoystick();
    }

    if(currentSelection == ACCELEROMETER){
        drawSelectionAccelerometer();
    }

    if(currentSelection == BLUETOOTH){
        drawSelectionBluetooth();
    }
}

#ifdef TEST
// global variable used in testCommands()
uint8_t test_command = 0;

// tests if commands sent are correct based on curr_val, forw_backw and right_left values
void testCommands()
{
    Interrupt_disableMaster();
    ADC14_disableConversion();

    test_command = 0;

    curr_val = 0;
    forw_backw = 1;
    right_left = 0;

    findCommand();

    if(test_command == 1)
    {
        printf("\nTest forward OK\n");
    }
    else
    {
        printf("\nTest forward FAILED\n");
    }

    forw_backw = -1;
    right_left = 0;

    findCommand();

    if(test_command == 2)
    {
        printf("Test backward OK\n");
    }
    else
    {
        printf("Test backward FAILED\n");
    }

    forw_backw = 0;
    right_left = 1;

    findCommand();

    if(test_command == 3)
    {
        printf("Test right OK\n");
    }
    else
    {
        printf("Test right FAILED\n");
    }

    forw_backw = 0;
    right_left = -1;
    findCommand();

    if(test_command == 4)
    {
        printf("Test left OK\n");
    }
    else
    {
        printf("Test left FAILED\n");
    }


    forw_backw = 0;
    right_left = 0;

    findCommand();

    if(test_command == 5)
    {
        printf("Test up OK\n\n");
    }
    else
    {
        printf("Test up FAILED\n\n");
    }
}

// tests if Bluetooth commands sent are the correct ones based on Buffer value
void testBluetooth()
{
    Buffer[0] = 'f';
    ble_command_manager();

    if(test_command == 1)
    {
        printf("\nBLE Test forward OK\n");
    }
    else
    {
        printf("\nBLE Test forward FAILED\n");
    }

    Buffer[0] = 'b';
    ble_command_manager();

    if(test_command == 2)
    {
        printf("BLE Test backward OK\n");
    }
    else
    {
        printf("BLE Test backward FAILED\n");
    }

    Buffer[0] = 'r';
    ble_command_manager();

    if(test_command == 3)
    {
        printf("BLE Test right OK\n");
    }
    else
    {
        printf("BLE Test right FAILED\n");
    }

    Buffer[0] = 'l';
    ble_command_manager();

    if(test_command == 4)
    {
        printf("BLE Test left OK\n");
    }
    else
    {
        printf("BLE Test left FAILED\n");
    }

    Buffer[0] = 'u';
    ble_command_manager();

    if(test_command == 5)
    {
        printf("BLE Test up OK\n");
    }
    else
    {
        printf("BLE Test up FAILED\n");
    }

    Buffer[0] = 'd';
    ble_command_manager();

    if(test_command == 5)
    {
        printf("BLE Test down OK\n");
    }
    else
    {
        printf("BLE Test down FAILED\n");
    }

    printf("\n Landing Test:\n");

    //a possible value for curr_val
    curr_val = 2;

    Buffer[0] = 'p';
    ble_command_manager();

    if(test_command == 5)
    {
        printf("BLE Test plane OK\n\n");
    }
    else
    {
        printf("BLE Test plane FAILED\n\n");
    }
}

#endif

