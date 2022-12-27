/*
 * direction_graphics.c
 *
 *  Created on: 14 dic 2022
 *      Author: damiano
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

#include <stdio.h>

#include "Hardware/Lcd/Crystalfontz128x128_ST7735.h"

#include "Hardware/Graphics/menu_graphics.h"
#include "Hardware/Graphics/images.h"
#include "Hardware/Graphics/imges_definitions.h"
#include "Hardware/Graphics/graphics_context.h"


const unsigned int PROPELLERS1_OFFSET_X = 8;
const unsigned int PROPELLERS1_OFFSET_Y = 30;
const unsigned int PROPELLERS2_OFFSET_X = 8;
const unsigned int PROPELLERS2_OFFSET_Y = 29;

const unsigned int MENU_OPTION_OFFSET_X = 17;
const unsigned int MENU_OPTION1_OFFSET_Y = 59;
const unsigned int MENU_OPTION2_OFFSET_Y = 75;
const unsigned int MENU_OPTION3_OFFSET_Y = 92;
const unsigned int BLUETOOTH_LOGO_OFFSET_X = 101;


const Graphics_Image MENU_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    menu_width,
    menu_height,
    menu_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_MENU_1BPP_UNCOMP,
};

const Graphics_Image JOYSTICK_BLUE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLUE_WHITE_1BPP_UNCOMP,
    pixel_JOYSTICK_1BPP_UNCOMP,
};

const Graphics_Image JOYSTICK_BLACK_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_JOYSTICK_1BPP_UNCOMP,
};

const Graphics_Image ACCELEROMETER_BLUE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLUE_WHITE_1BPP_UNCOMP,
    pixel_ACCELEROMETER_1BPP_UNCOMP,
};

const Graphics_Image ACCELEROMETER_BLACK_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_ACCELEROMETER_1BPP_UNCOMP,
};

const Graphics_Image BLUETOOTH_BLUE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLUE_WHITE_1BPP_UNCOMP,
    pixel_BLUETOOTH_1BPP_UNCOMP,
};

const Graphics_Image BLUETOOTH_BLACK_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_BLUETOOTH_1BPP_UNCOMP,
};

const Graphics_Image BLUETOOTH_LOGO_BLUE_8BPP_UNCOMP = {
    IMAGE_FMT_8BPP_UNCOMP,
    bluetooth_logo_width,
    bluetooth_logo_height,
    bluetooth_logo_size,
    palette_BLUETOOTH_LOGO_BLUE_8BPP_UNCOMP,
    pixel_BLUETOOTH_LOGO_8BPP_UNCOMP,
};

const Graphics_Image BLUETOOTH_LOGO_BLACK_8BPP_UNCOMP = {
    IMAGE_FMT_8BPP_UNCOMP,
    bluetooth_logo_width,
    bluetooth_logo_height,
    bluetooth_logo_size,
    palette_BLUETOOTH_LOGO_WHITE_8BPP_UNCOMP,
    pixel_BLUETOOTH_LOGO_8BPP_UNCOMP,
};


// draw the helicopter image at the startup of the system, with moving propellers at the end draws the menu
void startImageHelicopter()
{
    int j;
    Graphics_drawImage(&g_sContext, &HELICOPTER, 0, 0);
    customDelay(500000);

    // alternately draws the two propellers images, making the "movement" effect
    for(j=0; j<10; j++)
    {
        Graphics_drawImage(&g_sContext, &PROPELLERS1, PROPELLERS1_OFFSET_X, PROPELLERS1_OFFSET_Y);
        customDelay(300000);

        Graphics_drawImage(&g_sContext, &PROPELLERS2, PROPELLERS2_OFFSET_X, PROPELLERS2_OFFSET_Y);
        customDelay(300000);
    }

    Graphics_drawImage(&g_sContext, &MENU, 0, 0);
}

// delay using a counter 
void customDelay(int CYCLES)
{
    int j;

    //this could throw a warning: Detected SW delay loop using empty loop. Recommend using a timer module instead
    //but the creator has decided not to use another timer to implement this function
    for(j=0;j<CYCLES;j++){}
}


// move between the three menu options and redraws the option images based on the current selection
// the upper and lower bound for the y value received are set to 9800 and 7000 in order to ignore little and unwanted movements
void drawSelection(int y){

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

// redraw option images when JOYSTICK is selected
void drawSelectionJoystick(){
    Graphics_drawImage(&g_sContext, &JOYSTICK_BLUE, MENU_OPTION_OFFSET_X, MENU_OPTION1_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLACK, MENU_OPTION_OFFSET_X, MENU_OPTION2_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BLUETOOTH_BLACK, MENU_OPTION_OFFSET_X, MENU_OPTION3_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BLUETOOTH_LOGO_BLACK, BLUETOOTH_LOGO_OFFSET_X, MENU_OPTION3_OFFSET_Y+1);
}

// redraw option images when ACCELEROMETER is selected
void drawSelectionAccelerometer(){
    Graphics_drawImage(&g_sContext, &JOYSTICK_BLACK, MENU_OPTION_OFFSET_X, MENU_OPTION1_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLUE, MENU_OPTION_OFFSET_X, MENU_OPTION2_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BLUETOOTH_BLACK, MENU_OPTION_OFFSET_X, MENU_OPTION3_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BLUETOOTH_LOGO_BLACK, BLUETOOTH_LOGO_OFFSET_X, MENU_OPTION3_OFFSET_Y+1);
}

// redraw option images when BLUETOOTH is selected
void drawSelectionBluetooth(){
    Graphics_drawImage(&g_sContext, &JOYSTICK_BLACK, MENU_OPTION_OFFSET_X, MENU_OPTION1_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLACK, MENU_OPTION_OFFSET_X, MENU_OPTION2_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BLUETOOTH_BLUE, MENU_OPTION_OFFSET_X, MENU_OPTION3_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BLUETOOTH_LOGO_BLUE, BLUETOOTH_LOGO_OFFSET_X, MENU_OPTION3_OFFSET_Y+1);
}
