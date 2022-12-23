/*
 * HardwareInit.c
 *
 *  Created on: 30 nov 2022
 *      Author: damiano
 */

#include "LcdDriver/Crystalfontz128x128_ST7735.h"

#include "HardwareInit.h"
#include "init.h"



/* Statics */
#define TIMER_PERIOD    0x0BB8   //4000 to hex
#define TIMER_PERIOD2    0x07D0   //6000 to hex


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


//This function sets all pins of all ports to output mode and low value (some pins will be later overwritten by other Init functions) and should solve the warning of power consumption on unused pins
//EVEN IF the warning might still remain as this was reported to be a bug:
//see https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/535365/ulp4-1-issues-on-msp432 for further information
void _lowPowerInit()
{
    //P1
    GPIO_setAsOutputPin(GPIO_PORT_P1, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, PIN_ALL16);

    //P2
    GPIO_setAsOutputPin(GPIO_PORT_P2, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL16);

    //P3
    GPIO_setAsOutputPin(GPIO_PORT_P3, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, PIN_ALL16);

    //P4
    GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, PIN_ALL16);

    //P5
    GPIO_setAsOutputPin(GPIO_PORT_P5, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, PIN_ALL16);

    //P6
    GPIO_setAsOutputPin(GPIO_PORT_P6, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, PIN_ALL16);

    //P7
    GPIO_setAsOutputPin(GPIO_PORT_P7, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, PIN_ALL16);

    //P8
    GPIO_setAsOutputPin(GPIO_PORT_P8, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, PIN_ALL16);

    //P9
    GPIO_setAsOutputPin(GPIO_PORT_P9, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P9, PIN_ALL16);

    //P10
    GPIO_setAsOutputPin(GPIO_PORT_P10, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_P10, PIN_ALL16);

    //PJ
    GPIO_setAsOutputPin(GPIO_PORT_PJ, PIN_ALL16);
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ, PIN_ALL16);
}

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
void _joystickInit()
{
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with no repeat)
         * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);
}

//NOTE that ADC14_configureMultiSequenceMode(ADC_MEMx, ADC_MEMy, true); will "overwrite" a previous function of same type, so in order to have both
//Joystick and accelerometer working together you need to put ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true); so that both memory location are properly configured


//initialization of accelerometer (Pins and memory location where result of conversion has to be stored)
void _accelSensorInit()
{
    /* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory (ADC_MEM2 - ADC_MEM4 (A11, A13, A14)  with no repeat)
     * with 3.3v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true);
    ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM3, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM4, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 2 (end of sequence)
     *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT4);
}

//generic initialization of ADC14, to be used in conjunction with _adcInit() and/or _joystickInit()
void _adcInit()
{
    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);

    /* Setting up the sample timer to manually step through the sequence*/
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);


    /* Enabling ADC data conversion*/
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

    //currentScreen = HELICOPTER_GRAPHICS;

    int j, jj;
    Graphics_drawImage(&g_sContext, &HELICOPTER, 0, 0); for(j=0;j<CYCLES;j++){}
    for(jj=0; jj<20; jj++){
        Graphics_drawImage(&g_sContext, &PROPELLERS1, 8, 30); for(j=0;j<CYCLES/2;j++){}
        Graphics_drawImage(&g_sContext, &PROPELLERS2, 8, 29); for(j=0;j<CYCLES/2;j++){}
    }
    Graphics_drawImage(&g_sContext, &MENU, 0, 0); for(j=0;j<2*CYCLES;j++){}
    //currentScreen = MENU_GRAPHICS;

}
