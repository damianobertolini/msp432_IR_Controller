#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "directions.h"

extern const Graphics_Image DIRECTIONS_1BPP_UNCOMP;

extern const Graphics_Image HORIZONTAL_POWER_1BPP_UNCOMP;
extern const Graphics_Image VERTICAL_POWER_1BPP_UNCOMP;

extern const Graphics_Image FOREWARD_RED_1BPP_UNCOMP;
extern const Graphics_Image FOREWARD_WHITE_1BPP_UNCOMP;
extern const Graphics_Image BACKWARD_RED_1BPP_UNCOMP;
extern const Graphics_Image BACKWARD_WHITE_1BPP_UNCOMP;

extern const Graphics_Image LEFT_RED_1BPP_UNCOMP;
extern const Graphics_Image LEFT_WHITE_1BPP_UNCOMP;
extern const Graphics_Image RIGHT_RED_1BPP_UNCOMP;
extern const Graphics_Image RIGHT_WHITE_1BPP_UNCOMP;

extern const Graphics_Image HELICOPTER_8BPP_UNCOMP;
extern const Graphics_Image PROPELLERS1_8BPP_UNCOMP;
extern const Graphics_Image PROPELLERS2_8BPP_UNCOMP;

extern const Graphics_Image JOYSTICK_BLUE_1BPP_UNCOMP;
extern const Graphics_Image JOYSTICK_BLACK_1BPP_UNCOMP;
extern const Graphics_Image ACCELEROMETER_BLUE_1BPP_UNCOMP;
extern const Graphics_Image ACCELEROMETER_BLACK_1BPP_UNCOMP;
extern const Graphics_Image MENU_1BPP_UNCOMP;

#define DIRECTIONS DIRECTIONS_1BPP_UNCOMP

#define HORIZONTAL_POWER HORIZONTAL_POWER_1BPP_UNCOMP
#define VERTICAL_POWER VERTICAL_POWER_1BPP_UNCOMP

#define FOREWARD_RED FOREWARD_RED_1BPP_UNCOMP
#define FOREWARD_WHITE FOREWARD_WHITE_1BPP_UNCOMP
#define BACKWARD_RED BACKWARD_RED_1BPP_UNCOMP
#define BACKWARD_WHITE BACKWARD_WHITE_1BPP_UNCOMP

#define LEFT_RED LEFT_RED_1BPP_UNCOMP
#define LEFT_WHITE LEFT_WHITE_1BPP_UNCOMP
#define RIGHT_RED RIGHT_RED_1BPP_UNCOMP
#define RIGHT_WHITE RIGHT_WHITE_1BPP_UNCOMP
#define POWER_OFFSET 3

#define HELICOPTER HELICOPTER_8BPP_UNCOMP
#define PROPELLERS1 PROPELLERS1_8BPP_UNCOMP
#define PROPELLERS2 PROPELLERS2_8BPP_UNCOMP

#define JOYSTICK_BLUE JOYSTICK_BLUE_1BPP_UNCOMP
#define JOYSTICK_BLACK JOYSTICK_BLACK_1BPP_UNCOMP
#define ACCELEROMETER_BLUE ACCELEROMETER_BLUE_1BPP_UNCOMP
#define ACCELEROMETER_BLACK ACCELEROMETER_BLACK_1BPP_UNCOMP
#define MENU MENU_1BPP_UNCOMP

#define CYCLES 500000

typedef enum {NONE, JOYSTICK, ACCELEROMETER} Selection_t;
Selection_t currentSelection = NONE;    // keep track of the selection 


/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer[2];

void _adcInit(){
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    //Button S1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    //Button S2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);


    //enable interrupts for Port3 and Port5
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT3);


    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
            * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
        *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
        * convert.
        */
    ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

// Initialization function for graphics NOT IN USE
void initializeGraphics(Graphics_Context* g_sContext_p) {
    // Initialize the LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize context
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    // Set colors and fonts
    //Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    //Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_SLATE_BLUE);
    Graphics_setFont(g_sContext_p, &g_sFontCmss12);

    // Clear the screen
    //Graphics_clearDisplay(g_sContext_p);
}

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
    int j, jj;
    Graphics_drawImage(&g_sContext, &HELICOPTER, 0, 0); for(j=0;j<CYCLES;j++){}
    for(jj=0; jj<20; jj++){
        Graphics_drawImage(&g_sContext, &PROPELLERS1, 8, 30); for(j=0;j<CYCLES/2;j++){}
        Graphics_drawImage(&g_sContext, &PROPELLERS2, 8, 29); for(j=0;j<CYCLES/2;j++){}
    }
    Graphics_drawImage(&g_sContext, &MENU, 0, 0); for(j=0;j<2*CYCLES;j++){}
}

void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    _graphicsInit();
    _adcInit();

    //Button S1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);

    //enable interrupts for Port3 and Port5
    Interrupt_enableInterrupt(INT_PORT5);
}

int main(void)
{
    _hwInit();


    while(1)
    {
        PCM_gotoLPM0();
    }
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{

    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    int y_value;                            // integer that contains the y value of the joystick

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        y_value = (int) resultsBuffer[1];
        drawSelection(y_value);
    }
}

//Button S1 on BoosterPack Module
void PORT5_IRQHandler(void)
{
    printf("interrupt1\n");
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    printf("interrupt2\n");
    /* clear interrupt flag (to clear pending interrupt indicator */
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    printf("interrupt3\n");

    /* check if we received P5.1 interrupt */
    if((status & GPIO_PIN1)){
        if(currentSelection == JOYSTICK)
        {
            printf("joystick\n");
        }
        if(currentSelection == ACCELEROMETER)
        {
            printf("acc\n");
        }
    }
}

void drawSelection(int y){

    if(y>9800){
        currentSelection = JOYSTICK;
        printf("joystick\n");
    } else if(y<7000){
        currentSelection = ACCELEROMETER;
    }

    if(currentSelection == JOYSTICK){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLUE, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLACK, 17, 75);
    }

    if(currentSelection == ACCELEROMETER){
        Graphics_drawImage(&g_sContext, &JOYSTICK_BLACK, 17, 59);
        Graphics_drawImage(&g_sContext, &ACCELEROMETER_BLUE, 17, 75);
    }
    
}