/*
 * HardwareInit.c
 *
 *  Created on: 30 nov 2022
 *      Author: damiano
 */
#include "Hardware/hardware_init.h"

#include "Hardware/Bluetooth/Hardware/UART_Driver.h"
#include "Hardware/Bluetooth/Controller/UART_IO.h"

#include "Hardware/Lcd/Crystalfontz128x128_ST7735.h"
#include "Hardware/Graphics/images_definitions.h"
#include "Hardware/Graphics/menu_graphics.h"


/*Data Buffer*/
char Buffer[BLUETOOTH_BUFFER_SIZE];


/* Timer_A UpMode Configuration Parameters (Timer A1, A2, A3)*/
const Timer_A_UpModeConfig upConfigTimerA3 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // ACLK/1 = 32768 Hz
        TIMER_PERIOD3,                          // every second
        TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,   // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                       // Clear value
};

const Timer_A_UpModeConfig upConfigTimerA2 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // ACLK/1 = 32768 Hz
        TIMER_PERIOD2,                         // every second
        TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,   // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                       // Clear value
};

const Timer_A_UpModeConfig upConfigTimerA1 =
{
        TIMER_A_CLOCKSOURCE_ACLK,              // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // ACLK/1 = 32768 Hz
        TIMER_PERIOD1,                         // every second
        TIMER_A_TAIE_INTERRUPT_DISABLE,        // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,   // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                       // Clear value
};


/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate with
 * System Clock operating at 48 MHz.
 * These values were calculated using the online calculator that TI provides at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */

const eUSCI_UART_ConfigV1 UART2Config =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     78,
     2,
     0,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
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

void _PCM_Flash_WDT_Init()
{
    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

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
        Timer_A_configureUpMode(TIMER_A1_BASE, &upConfigTimerA1);


        /* Enabling interrupts and starting the timer */
        //Interrupt_enableSleepOnIsrExit();
        Interrupt_enableInterrupt(INT_TA3_0);
        Interrupt_enableInterrupt(INT_TA2_0);
        Interrupt_enableInterrupt(INT_TA1_0);
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

    //Joystick button
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);

    //enable interrupts for Port3 and Port5 and Port4
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT4);
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
    //note that I'm also including ADC_MEM0 and ADC_MEM1 (the memory registers used by ADC for Joystick) as this allows to use both accelerometer and Joystick together
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

    /* Setting up the sample timer to manually step through the sequence*/
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Enabling ADC data conversion*/
    ADC14_enableConversion();

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
}

//initialization of the Bluetooth connection through UART
void _bluetoothInit()
{
    // Note that Bluetooth requires the clock to be working at 48 Mhz (CS_setDCOCenteredFrequency()) and _PCM_Flash_Init()

    /*Initialize Hardware required for the HC-05*/
    UART_Init(EUSCI_A2_BASE, UART2Config);     //configure Bluetooth in order to operate at 9600 baud rate
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
}

