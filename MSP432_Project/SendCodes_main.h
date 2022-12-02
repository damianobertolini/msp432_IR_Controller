/*
 * SendCodes_main.h
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#ifndef SENDCODES_MAIN_H_
#define SENDCODES_MAIN_H_

// Length of the commands
#define MAX_PULSE 34


void PWM_Init(float period, float duty);
void enablePWM(void);
void disablePWM(void);
void disablePWM(void);
void SysTick_Init(void);
void mark(unsigned int usec);
void space(unsigned int usec);
void sendCommand(unsigned int cmd[], unsigned int pause[], unsigned int length);
void _initPWM(void);

#endif /* SENDCODES_MAIN_H_ */
