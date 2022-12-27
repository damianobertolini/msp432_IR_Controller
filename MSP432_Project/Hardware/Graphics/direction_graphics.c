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
#include "Hardware/Graphics/imges_definitions.h"
#include "Hardware/Graphics/graphics_context.h"

typedef enum {NONE, FORWARD, BACKWARD, LEFT, RIGHT} Direction_t;

// keep track of the direction during the execution to avoid useless redrawings
Direction_t currentDirection = NONE;


const unsigned int RIGHT_DIRECTION_OFFSET_X = 76;
const unsigned int RIGHT_DIRECTION_OFFSET_Y = 46;

const unsigned int LEFT_DIRECTION_OFFSET_X = 8;
const unsigned int LEFT_DIRECTION_OFFSET_Y = 46;

const unsigned int BACKWARD_DIRECTION_OFFSET_X = 46;
const unsigned int BACKWARD_DIRECTION_OFFSET_Y = 76;

const unsigned int FORWARD_DIRECTION_OFFSET_X = 46;
const unsigned int FORWARD_DIRECTION_OFFSET_Y = 8;

const unsigned int HORIZONTAL_POWER_OFFSET_X = 58;
const unsigned int VERTICAL_POWER_OFFSET_Y = 58;
const unsigned int RIGHT_POWER_OFFSET_X = 78;
const unsigned int LEFT_POWER_OFFSET_X = 47;
const unsigned int FORWARD_POWER_OFFSET_Y = 47;
const unsigned int BACKWARD_POWER_OFFSET_Y = 78;


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


const tImage HELICOPTER_8BPP_UNCOMP = {
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

const tImage FORWARD_RED_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    FORWARD_width,
    FORWARD_height,
    FORWARD_size,
    palette_BLACK_RED_1BPP_UNCOMP,
    pixel_FORWARD_1BPP_UNCOMP,
};

const tImage FORWARD_WHITE_1BPP_UNCOMP = {
    IMAGE_FMT_1BPP_UNCOMP,
    FORWARD_width,
    FORWARD_height,
    FORWARD_size,
    palette_BLACK_WHITE_1BPP_UNCOMP,
    pixel_FORWARD_1BPP_UNCOMP,
};


// redraw the directions and their respective power levels
void drawDirections(int x, int y, Selection_t currentModality){
    int sign_x_value = 0;               // 1, 0, -1 whether it's right, none, left direction
    int sign_y_value = 0;               // 1, 0, -1 whether it's forward, none, backward direction

    bool greater_module_x;
    int left_right_power = 0;           // number of power levels for left/right direction
    int forward_backward_power = 0;     // number of power levels for forward/backward direction

    /*
      since the values (x, y) received by the draw directions function are different in the case of the joystick  or the accelerometer,
      it is necessary to redefine the lower bound and upper bound for both directions in the two cases considered

      - limit values received form the joystick:        x:[0, 16300],    y[0, 16300]
      - limit values received form the accelerometer:   x:[5000, 11400], y[5000, 11400]

      the "forward" or "backward" direction is considered selected only if the y value:
      - joystick:       forward y>9800,     backward y<7000
      - accelerometer:  forward y>8800,     backward y<7600

      the "left" or "right" direction is considered selected only if the y value:
      - joystick:       left x<7000,    right x>9800
      - accelerometer:  left x<7600,    right x>8800

      the power level is calculated by dividing the received value by a suitable number, so that the maximum number of power level is 5,
      in this regard we divide values (x, y) by:
      - joystick:       1400
      - accelerometer:  600
    */
    // set parameters in order to choose the current direction and get the power levels

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

    // determine which is the most powerful direction: left/right or forward/backward
    if(module_value(x-8200)>module_value(y-8200)){greater_module_x=true;} else {greater_module_x=false;}

    // determine which (and if) horizontal direction is colored red
    if(greater_module_x){
        if(sign_x_value == 1){
            if(currentDirection!=RIGHT){
                drawDirectionRight();
                currentDirection = RIGHT;
            }
            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
        if(sign_x_value == 0){
            drawDirectionNone();
            currentDirection = NONE;
        }
        if(sign_x_value == -1){
            if(currentDirection!=LEFT){
                drawDirectionLeft();
                currentDirection = LEFT;
            }
            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
    }

    // determine which (and if) vertical direction is colored red
    if(!greater_module_x){
        if(sign_y_value == 1){
            if(currentDirection!=FORWARD){
                drawDirectionForward();
                currentDirection = FORWARD;
            }
            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
        if(sign_y_value == 0){
            drawDirectionNone();
            currentDirection = NONE;
        }
        if(sign_y_value == -1){
            if(currentDirection!=BACKWARD){
                drawDirectionBackward();
                currentDirection = BACKWARD;
            }
            drawPower(left_right_power, forward_backward_power, sign_x_value, sign_y_value);
        }
    }

}

// draw the power levels on both horizontal and vertical directions
void drawPower(int left_right_power, int FORWARD_backward_power, int sign_x_value, int sign_y_value){
    drawVerticalPower(sign_x_value, left_right_power);
    drawHorizontalPower(sign_y_value, FORWARD_backward_power);
}

// draw the power levels on vertical directions
void drawHorizontalPower(int sign_y_value, int FORWARD_backward_power) {
    int i;

    if(sign_y_value == 1){
        for(i=0; i<FORWARD_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, HORIZONTAL_POWER_OFFSET_X, FORWARD_POWER_OFFSET_Y-(POWER_OFFSET*i));
        }
    } else if(sign_y_value == -1){
        for(i=0; i<FORWARD_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, HORIZONTAL_POWER_OFFSET_X, BACKWARD_POWER_OFFSET_Y+(POWER_OFFSET*i));
        }
    }
}

// draw the power levels on horizontal directions
void drawVerticalPower(int sign_x_value, int left_right_power) {
    int i;

    if(sign_x_value == 1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, RIGHT_POWER_OFFSET_X+(POWER_OFFSET*i), VERTICAL_POWER_OFFSET_Y);
        }
    } else if(sign_x_value == -1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, LEFT_POWER_OFFSET_X-(POWER_OFFSET*i), VERTICAL_POWER_OFFSET_Y);
        }
    }
}

int module_value(int a){
    if(a<0) return -a;
    return a;
}

// redraw directio images when the current direction is FORWARD
void drawDirectionForward(){
    Graphics_drawImage(&g_sContext, &FORWARD_RED, FORWARD_DIRECTION_OFFSET_X, FORWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, BACKWARD_DIRECTION_OFFSET_X, BACKWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &LEFT_WHITE, LEFT_DIRECTION_OFFSET_X, LEFT_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &RIGHT_WHITE, RIGHT_DIRECTION_OFFSET_X, RIGHT_DIRECTION_OFFSET_Y);
}

// redraw directio images when the current direction is BACKWARD
void drawDirectionBackward(){
    Graphics_drawImage(&g_sContext, &FORWARD_WHITE, FORWARD_DIRECTION_OFFSET_X, FORWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BACKWARD_RED, BACKWARD_DIRECTION_OFFSET_X, BACKWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &LEFT_WHITE, LEFT_DIRECTION_OFFSET_X, LEFT_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &RIGHT_WHITE, RIGHT_DIRECTION_OFFSET_X, RIGHT_DIRECTION_OFFSET_Y);
}

// redraw directio images when the current direction is LEFT
void drawDirectionLeft(){
    Graphics_drawImage(&g_sContext, &FORWARD_WHITE, FORWARD_DIRECTION_OFFSET_X, FORWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, BACKWARD_DIRECTION_OFFSET_X, BACKWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &LEFT_RED, LEFT_DIRECTION_OFFSET_X, LEFT_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &RIGHT_WHITE, RIGHT_DIRECTION_OFFSET_X, RIGHT_DIRECTION_OFFSET_Y);
}

// redraw directio images when the current direction is RIGHT
void drawDirectionRight(){
    Graphics_drawImage(&g_sContext, &FORWARD_WHITE, FORWARD_DIRECTION_OFFSET_X, FORWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, BACKWARD_DIRECTION_OFFSET_X, BACKWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &LEFT_WHITE, LEFT_DIRECTION_OFFSET_X, LEFT_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &RIGHT_RED, RIGHT_DIRECTION_OFFSET_X, RIGHT_DIRECTION_OFFSET_Y);
}

// redraw directio images when the current direction is NONE
void drawDirectionNone(){
    Graphics_drawImage(&g_sContext, &FORWARD_WHITE, FORWARD_DIRECTION_OFFSET_X, FORWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, BACKWARD_DIRECTION_OFFSET_X, BACKWARD_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &LEFT_WHITE, LEFT_DIRECTION_OFFSET_X, LEFT_DIRECTION_OFFSET_Y);
    Graphics_drawImage(&g_sContext, &RIGHT_WHITE, RIGHT_DIRECTION_OFFSET_X, RIGHT_DIRECTION_OFFSET_Y);
}
