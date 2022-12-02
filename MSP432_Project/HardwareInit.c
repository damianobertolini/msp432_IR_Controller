/*
 * HardwareInit.c
 *
 *  Created on: 30 nov 2022
 *      Author: damiano
 */

#include "LcdDriver/Crystalfontz128x128_ST7735.h"

#include "HardwareInit.h"



/* Statics */
#define TIMER_PERIOD    0x0FA0   //4000 to hex
#define TIMER_PERIOD2    0x1770   //6000 to hex


/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfigTimerA3 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

const Timer_A_UpModeConfig upConfigTimerA2 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD2,                           // every half second
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};



void _PCM_Flash_Init()
{
    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
}


void _ledInit()
{
    /* Configuring P1.0 as output */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}


void _timersInit()
{
        /* Configuring Timer_A1 for Up Mode */
        Timer_A_configureUpMode(TIMER_A3_BASE, &upConfigTimerA3);
        Timer_A_configureUpMode(TIMER_A2_BASE, &upConfigTimerA2);

        /* Enabling interrupts and starting the timer */
        //Interrupt_enableSleepOnIsrExit();
        Interrupt_enableInterrupt(INT_TA3_0);
        Interrupt_enableInterrupt(INT_TA2_0);
}


//Buttons on TI J4.32 and J4.33
void _buttonsInit()
{
    //Button S1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    //Button S2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);


    //enable interrupts for Port3 and Port5
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT3);
}



//initialize Analog to Digital Converter for Joystick and enables its interrupts
void _adcInit()
{
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
         * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);

    /* Setting up the sample timer to manually step through the sequence*/
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
}


//initialize Liquid Crystal Display
void _graphicsInit()
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

    Graphics_drawStringCentered(&g_sContext,
        (int8_t *)"Joystick:",
        AUTO_STRING_LENGTH,
        64,
        30,
        OPAQUE_TEXT);

}

