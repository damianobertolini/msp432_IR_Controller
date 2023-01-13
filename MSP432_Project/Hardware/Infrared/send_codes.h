/*
 * SendCodes_main.h
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#ifndef SEND_CODES_H_
#define SEND_CODES_H_

// EFFECT: initializes the PWM to send IR codes on P2.5
//         duty can be 0-1 (0-100%)
//         period is in seconds
void PWM_Init(float period, float duty);

// EFFECT: sets the MC bits of TimerA0 to enable the PWM pulse
void enablePWM(void);

// EFFECT: clears the MC bits of TimerA0 to disable the PWM pulse
void disablePWM(void);

// EFFECT: initializes SysTick to time pulses
void SysTick_Init(void);

// EFFECT: calls enablePWM() for usec microseconds and then calls disablePWM()
void mark(uint16_t usec);

// EFFECT: does nothing (waits) for usec
void space(uint16_t usec);

// EFFECT: sends the array of pulses cmd[] with pauseTimes[] between each pulse
void sendCommand(const uint16_t cmd[], const uint16_t pause[], uint8_t length);

// EFFECT: calls all initialization functions
void _initPWM(void);

#endif /* SEND_CODES_H_ */
