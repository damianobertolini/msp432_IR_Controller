/*
 * buttons_interrupt.c
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#include "interrupts.h"


int mode_selected = 0;


// Joystick button on BoosterPack Module
// when it is pressed, if the user is in menu mode, the currentSelection modality is selected and will be used to drive the helicopter
void PORT4_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    /* check if we received P3.5 interrupt */
    if((status & GPIO_PIN1)){
        //user has chosen modality
        if(!mode_selected)
        {
            //not modifiable anymore
            mode_selected = 1;

            if(currentSelection == BLUETOOTH)
            {
                MSPrintf(EUSCI_A2_BASE, "Connection OK       ");
                MSPrintf(EUSCI_A2_BASE, "Waiting for data    ");
            }

            Graphics_drawImage(&g_sContext, &DIRECTIONS, 0, 0);

            //start A1 timer for directions drawing (TIMER_A1)
            Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
        }
    }
}


//Button S2 on BoosterPack Module
//note that strangely PORT3 has an interrupt at the start of execution process, seemingly not related to a particular PIN
void PORT3_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    /* clear interrupt flag (to clear pending interrupt indicator */

    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    /* check if we received P3.5 interrupt */
    if((status & GPIO_PIN5)){
        if(curr_val == 0)
        {
            //won't decrement as velocity matrix index can't go under 0 (vector[-1] doesn't exist)
        }
        else
        {
            //decrease speed
            curr_val--;
        }
    }
}


//Button S1 on BoosterPack Module
void PORT5_IRQHandler(void)
{
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    /* check if we received P5.1 interrupt */
    if((status & GPIO_PIN1)){
        if(curr_val == 4)
        {
            //won't increment as velocity matrix only has 5 possible positions
        }
        else
        {
            //increase speed
            curr_val++;
        }
    }
}
