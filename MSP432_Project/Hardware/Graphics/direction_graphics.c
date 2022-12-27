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

#include "Hardware/Graphics/direction_graphics.h"
#include "Hardware/Graphics/images.h"
#include "Hardware/Graphics/img_vars.h"



const unsigned int RIGHT_DIRECTION_OFFSET_X = 76;
const unsigned int RIGHT_DIRECTION_OFFSET_Y = 46;

const unsigned int LEFT_DIRECTION_OFFSET_X = 8;
const unsigned int LEFT_DIRECTION_OFFSET_Y = 46;

const unsigned int BACKWARD_DIRECTION_OFFSET_X = 46;
const unsigned int BACKWARD_DIRECTION_OFFSET_Y = 76;

const unsigned int FOREWARD_DIRECTION_OFFSET_X = 46;
const unsigned int FOREWARD_DIRECTION_OFFSET_Y = 8;

const unsigned int HORIZONTAL_POWER_OFFSET_X = 58;
const unsigned int FOREWARD_POWER_OFFSET_Y = 44;
const unsigned int BACKWARD_POWER_OFFSET_Y = 78;

const unsigned int VERTICAL_POWER_OFFSET_Y = 58;
const unsigned int RIGHT_POWER_OFFSET_X = 78;
const unsigned int LEFT_POWER_OFFSET_X = 44;


const unsigned int PROPELLERS1_OFFSET_X = 8;
const unsigned int PROPELLERS1_OFFSET_Y = 30;
const unsigned int PROPELLERS2_OFFSET_X = 5;
const unsigned int PROPELLERS2_OFFSET_Y = 30;

const unsigned int MENU_OPTION_OFFSET_X = 17;
const unsigned int MENU_OPTION1_OFFSET_Y = 59;
const unsigned int MENU_OPTION2_OFFSET_Y = 75;



const Graphics_Image JOYSTICK_BLACK_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_JOYSTICK_1BPP_UNCOMP,
};


const Graphics_Image JOYSTICK_BLUE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    option_width,
    option_height,
    option_size,
    palette_BLUE_WHITE_1BPP_UNCOMP,
    pixel_JOYSTICK_1BPP_UNCOMP,
};

const Graphics_Image MENU_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    menu_width,
    menu_height,
    menu_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_MENU_1BPP_UNCOMP,
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

const tImage HORIZONTAL_POWER_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    power_width,
    power_height,
    power_size,
    palette_BLACK_1BPP_UNCOMP,
    pixel_HORIZONTAL_POWER_1BPP_UNCOMP,
};

const tImage VERTICAL_POWER_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    power_height,
    power_width,
    power_size,
    palette_BLACK_1BPP_UNCOMP,
    pixel_VERTICAL_POWER_1BPP_UNCOMP,
};

const tImage BACKWARD_RED_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    backward_width,
    backward_height,
    backward_size,
    palette_BLACK_RED_1BPP_UNCOMP,
    pixel_BACKWARD_1BPP_UNCOMP,
};

const tImage BACKWARD_WHITE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    backward_width,
    backward_height,
    backward_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_BACKWARD_1BPP_UNCOMP,
};

const tImage LEFT_RED_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    left_width,
    left_height,
    left_size,
    palette_BLACK_RED_1BPP_UNCOMP,
    pixel_LEFT_1BPP_UNCOMP,
};

const tImage LEFT_WHITE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    left_width,
    left_height,
    left_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_LEFT_1BPP_UNCOMP,
};

const tImage RIGHT_RED_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    right_width,
    right_height,
    right_size,
    palette_BLACK_RED_1BPP_UNCOMP,
    pixel_RIGHT_1BPP_UNCOMP,
};

const tImage RIGHT_WHITE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    right_width,
    right_height,
    right_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_RIGHT_1BPP_UNCOMP,
};

const tImage DIRECTIONS_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    directions_width,
    directions_height,
    directions_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_DIRECTIONS_1BPP_UNCOMP,
};


const tImage HELICOPTER_8BPP_UNCOMP={
    IMAGE_FMT_8BPP_UNCOMP,
    helicopter_width,
    helicopter_height,
    helicopter_size,
    palette_HELICOPTER_8BPP_UNCOMP,
    pixel_HELICOPTER_8BPP_UNCOMP,
};

const tImage PROPELLERS1_8BPP_UNCOMP = {
    IMAGE_FMT_8BPP_UNCOMP,
    propellers1_width,
    propellers1_height,
    helicopter_size,
    palette_HELICOPTER_8BPP_UNCOMP,
    pixel_PROPELLERS1_8BPP_UNCOMP,
};

const tImage PROPELLERS2_8BPP_UNCOMP = {
    IMAGE_FMT_8BPP_UNCOMP,
    propellers2_width,
    propellers2_height,
    helicopter_size,
    palette_HELICOPTER_8BPP_UNCOMP,
    pixel_PROPELLERS2_8BPP_UNCOMP,
};

const tImage FOREWARD_RED_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    foreward_width,
    foreward_height,
    foreward_size,
    palette_BLACK_RED_1BPP_UNCOMP,
    pixel_FOREWARD_1BPP_UNCOMP,
};

const tImage FOREWARD_WHITE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    foreward_width,
    foreward_height,
    foreward_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_FOREWARD_1BPP_UNCOMP,
};


typedef enum {NONE, FOREWARD, BACKWARD, LEFT, RIGHT} Direction_t;

Direction_t currentDirection = NONE;    // keep track of the direction during the execution to avoid useless redrawings

/* Graphic library context */
Graphics_Context g_sContext;


void drawHorizontalPower(int sign_y_value, int foreward_backward_power) {
    int i;

    if(sign_y_value == 1){
        for(i=0; i<foreward_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 47-(POWER_OFFSET*i));
        }
    } else if(sign_y_value == -1){
        for(i=0; i<foreward_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 78+(POWER_OFFSET*i));
        }
    }
}

void drawVerticalPower(int sign_x_value, int left_right_power) {
    int i;

    if(sign_x_value == 1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 78+(POWER_OFFSET*i), 58);
        }
    } else if(sign_x_value == -1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 47-(POWER_OFFSET*i), 58);
        }
    }
}


void drawPower(int left_right_power, int foreward_backward_power, int sign_x_value, int sign_y_value){
    int i;

    if(sign_x_value == 1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 78+(POWER_OFFSET*i), 58);
        }
        drawHorizontalPower(sign_y_value, foreward_backward_power);
    }

    if(sign_x_value == -1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 47-(POWER_OFFSET*i), 58);
        }
        drawHorizontalPower(sign_y_value, foreward_backward_power);
    }

    if(sign_y_value == 1){
        for(i=0; i<foreward_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 47-(POWER_OFFSET*i));
        }
        drawVerticalPower(sign_x_value, left_right_power);
    }

    if(sign_y_value == -1){
        for(i=0; i<foreward_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 78+(POWER_OFFSET*i));
        }
        drawVerticalPower(sign_x_value, left_right_power);
    }
}


int module_value(a){
    if(a<0) return -a;
    return a;
}


void drawDirections(int x, int y, Selection_t currentModality){
    int sign_x_value = 0;               // 1, 0, -1 whether it's right, none, left direction
    int sign_y_value = 0;               // 1, 0, -1 whether it's forward, none, backward direction

    bool greater_module_x;
    int left_right_power = 0;           // number of power levels for left/right direction
    int forward_backward_power = 0;     // number of power levels for forward/backward direction

    //set parameters in order to choose the current direction and get the power levels

    if(currentModality == JOYSTICK){
        if(x>9800){
            sign_x_value = 1;
            left_right_power = (int) (x-8200)/1400;
        } else if(x<7000){
            sign_x_value = -1;
            left_right_power = (int) (8200-x)/1400;
        }

        if(y>9800){
            sign_y_value = 1;
            forward_backward_power = (int) (y-8200)/1400;
        } else if(y<7000){
            sign_y_value = -1;
            forward_backward_power = (int) (8200-y)/1400;
        }
    }

    if(currentModality == ACCELEROMETER){
        if(x>8800){
            sign_x_value = 1;
            left_right_power = (int) (x-8200)/600;
        } else if(x<7600){
            sign_x_value = -1;
            left_right_power = (int) (8200-x)/600;
        }

        if(y>8800){
            sign_y_value = 1;
            forward_backward_power = (int) (y-8200)/600;
        } else if(y<7600){
            sign_y_value = -1;
            forward_backward_power = (int) (8200-y)/600;
        }
    }

    // which is the most powerful direction: left/right or forward/backward
    if(module_value(x-8200)>module_value(y-8200)){greater_module_x=true;} else {greater_module_x=false;}

    // left or right direction
    if(greater_module_x){
        if(sign_x_value == 1){
            if(currentDirection!=RIGHT){
                Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
                Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
                Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
                Graphics_drawImage(&g_sContext, &RIGHT_RED, 76, 46);
                currentDirection = RIGHT;
            }
            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
        if(sign_x_value == 0){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
            currentDirection = NONE;
        }
        if(sign_x_value == -1){
            if(currentDirection!=LEFT){
                Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
                Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
                Graphics_drawImage(&g_sContext, &LEFT_RED, 8, 46);
                Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
                currentDirection = LEFT;
            }

            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
    }

    // foreward or backward direction
    if(!greater_module_x){
        if(sign_y_value == 1){
            if(currentDirection!=FOREWARD){
                Graphics_drawImage(&g_sContext, &FOREWARD_RED, 46, 8);
                Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
                Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
                Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
                currentDirection = FOREWARD;
            }

            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
        if(sign_y_value == 0){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
            currentDirection = NONE;
        }
        if(sign_y_value == -1){
            if(currentDirection!=BACKWARD){
                Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
                Graphics_drawImage(&g_sContext, &BACKWARD_RED, 46, 76);
                Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
                Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
                currentDirection = BACKWARD;
            }

            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
    }

}



