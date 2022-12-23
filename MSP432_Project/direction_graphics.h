/*
 * direction_graphics.h
 *
 *  Created on: 14 dic 2022
 *      Author: damiano
 */

#ifndef DIRECTION_GRAPHICS_H_
#define DIRECTION_GRAPHICS_H_

typedef enum {JOYSTICK, ACCELEROMETER} Selection_t;


void drawDirections(int x, int y, Selection_t currentModality);


#endif /* DIRECTION_GRAPHICS_H_ */
