/*
 * SendCodes_main.h
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#ifndef SEND_CODES_H_
#define SEND_CODES_H_


void PWM_Init(float period, float duty);
void enablePWM(void);
void disablePWM(void);
void SysTick_Init(void);
void mark(unsigned int usec);
void space(unsigned int usec);
void sendCommand(const unsigned int cmd[], const unsigned int pause[], unsigned int length);
void _initPWM(void);

#endif /* SEND_CODES_H_ */
