/*
 * adc_interrupt.c
 *
 *  Created on: 27 dic 2022
 *      Author: damiano
 */

#include "interrupts.h"

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM0 to ADC_MEM4 (depending on selected modality). This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    /* ADC results buffer that can store both the Joystick results and accelerometer ones */
    uint16_t resultsBuffer[3];

    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    right_left = 0; //reset right-left state (stop turning)
    forw_backw = 0;  //reset forw-backw state (stop moving)


    //still in menu mode
    if(!mode_selected)
    {
        //joystick
        if(status & ADC_INT1)
        {
            /* Store ADC14 conversion results */
            resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

            //switch modality in menu
            drawSelection((int) resultsBuffer[1]);
        }

        return;
    }


    int x_value;                            // integer that contains the x value of the joystick
    int y_value;                            // integer that contains the y value of the joystick


    /* ADC_MEM1 or ADC_MEM2 conversion completed */
    //joystick data ready

    if((status & ADC_INT1) && currentSelection == JOYSTICK)
    {
        /* Store ADC14 conversion results */
        // Joystick results are stored in ADC_MEM0 and ADC_MEM1
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        //draw Direction power
        x_value = (int) resultsBuffer[0];
        y_value = (int) resultsBuffer[1];


        //manipulate Joystick X position and decide accordingly whether to turn left, right or stay still


        if(x_value > 14000)
        {
            right_left = 1;  //right
        }
        if(x_value < 2000)
        {
            right_left = -1;  //left
        }


        //manipulate Joystick Y position and decide accordingly whether to move forward, backward or stay still

        if(y_value > 14000)
        {
            forw_backw = 1;  //forward
        }
        if(y_value < 2000)
        {
            forw_backw = -1; //backward
        }

        drawDirections(x_value, y_value, currentSelection);

        //decide which command to send and emit IR signals accordingly
        findCommand();
    }
    else
    {
        //accelerometer data ready
        if ((status & ADC_INT4) && currentSelection == ACCELEROMETER)
        {

            /* Store ADC14 conversion results */
            // Accelerometer results are stored in ADC_MEM2, ADC_MEM3 and ADC_MEM4
            resultsBuffer[0] = ADC14_getResult(ADC_MEM2);
            resultsBuffer[1] = ADC14_getResult(ADC_MEM3);
            resultsBuffer[2] = ADC14_getResult(ADC_MEM4);


            //draw Direction power
            x_value = (int) resultsBuffer[0];
            y_value = (int) resultsBuffer[1];


            if(x_value < 6500)
            {
                right_left = -1;
            }
            if(x_value > 9800)
            {
                right_left = 1;
            }

            if(y_value < 7000)
            {
                forw_backw = -1;
            }
            if(y_value > 9600)
            {
                forw_backw = 1;
            }

            drawDirections(x_value, y_value, currentSelection);

            //decide which command to send and emit IR signals accordingly
            findCommand();
        }
    }
}
