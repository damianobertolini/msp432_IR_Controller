/*
 * img_vars.h
 *
 *  Created on: 13 dic 2022
 *      Author: damiano
 */

#ifndef IMG_VARS_H_
#define IMG_VARS_H_

#include <ti/grlib/grlib.h>


extern const Graphics_Image DIRECTIONS_1BPP_UNCOMP;

extern const Graphics_Image HORIZONTAL_POWER_1BPP_UNCOMP;
extern const Graphics_Image VERTICAL_POWER_1BPP_UNCOMP;

extern const Graphics_Image FOREWARD_RED_1BPP_UNCOMP;
extern const Graphics_Image FOREWARD_WHITE_1BPP_UNCOMP;
extern const Graphics_Image BACKWARD_RED_1BPP_UNCOMP;
extern const Graphics_Image BACKWARD_WHITE_1BPP_UNCOMP;

extern const Graphics_Image LEFT_RED_1BPP_UNCOMP;
extern const Graphics_Image LEFT_WHITE_1BPP_UNCOMP;
extern const Graphics_Image RIGHT_RED_1BPP_UNCOMP;
extern const Graphics_Image RIGHT_WHITE_1BPP_UNCOMP;

extern const Graphics_Image HELICOPTER_8BPP_UNCOMP;
extern const Graphics_Image PROPELLERS1_8BPP_UNCOMP;
extern const Graphics_Image PROPELLERS2_8BPP_UNCOMP;

extern const Graphics_Image JOYSTICK_BLUE_1BPP_UNCOMP;
extern const Graphics_Image JOYSTICK_BLACK_1BPP_UNCOMP;
extern const Graphics_Image ACCELEROMETER_BLUE_1BPP_UNCOMP;
extern const Graphics_Image ACCELEROMETER_BLACK_1BPP_UNCOMP;
extern const Graphics_Image BLUETOOTH_BLUE_8BPP_UNCOMP;
extern const Graphics_Image BLUETOOTH_BLACK_8BPP_UNCOMP;
extern const Graphics_Image MENU_1BPP_UNCOMP;

#define DIRECTIONS DIRECTIONS_1BPP_UNCOMP

#define HORIZONTAL_POWER HORIZONTAL_POWER_1BPP_UNCOMP
#define VERTICAL_POWER VERTICAL_POWER_1BPP_UNCOMP

#define FOREWARD_RED FOREWARD_RED_1BPP_UNCOMP
#define FOREWARD_WHITE FOREWARD_WHITE_1BPP_UNCOMP
#define BACKWARD_RED BACKWARD_RED_1BPP_UNCOMP
#define BACKWARD_WHITE BACKWARD_WHITE_1BPP_UNCOMP

#define LEFT_RED LEFT_RED_1BPP_UNCOMP
#define LEFT_WHITE LEFT_WHITE_1BPP_UNCOMP
#define RIGHT_RED RIGHT_RED_1BPP_UNCOMP
#define RIGHT_WHITE RIGHT_WHITE_1BPP_UNCOMP
#define POWER_OFFSET 3

#define HELICOPTER HELICOPTER_8BPP_UNCOMP
#define PROPELLERS1 PROPELLERS1_8BPP_UNCOMP
#define PROPELLERS2 PROPELLERS2_8BPP_UNCOMP

#define JOYSTICK_BLUE JOYSTICK_BLUE_1BPP_UNCOMP
#define JOYSTICK_BLACK JOYSTICK_BLACK_1BPP_UNCOMP
#define ACCELEROMETER_BLUE ACCELEROMETER_BLUE_1BPP_UNCOMP
#define ACCELEROMETER_BLACK ACCELEROMETER_BLACK_1BPP_UNCOMP
#define MENU MENU_1BPP_UNCOMP

#define CYCLES 500000

//typedef enum {HELICOPTER_GRAPHICS, MENU_GRAPHICS, DIRECTIONS_GRAPHICS} Screen_t;



#endif /* IMG_VARS_H_ */
