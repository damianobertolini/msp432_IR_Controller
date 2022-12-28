/*
 * direction_graphics.h
 *
 *  Created on: 14 dic 2022
 *      Author: damiano
 */

#ifndef DIRECTION_GRAPHICS_H_
#define DIRECTION_GRAPHICS_H_

#include "Hardware/Graphics/menu_graphics.h"

void drawDirections(int x, int y, Selection_t currentModality);
void drawPower(int left_right_power, int forward_backward_power, int sign_x_value, int sign_y_value);
void drawHorizontalPower(int sign_y_value, int forward_backward_power);
void drawVerticalPower(int sign_x_value, int left_right_power);
void drawDirectionForward();
void drawDirectionBackward();
void drawDirectionLeft();
void drawDirectionRight();
void drawDirectionNone();
void testDirectionGraphics1();
void testDirectionGraphics2();

#endif /* DIRECTION_GRAPHICS_H_ */
