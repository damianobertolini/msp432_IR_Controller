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


/* Graphic library context */
Graphics_Context g_sContext;

/* ADC results buffer */
static uint16_t resultsBuffer[2];

void _adcInit(){
    /* Configures Pin 6.0 and 4.4 as ADC input */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

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
        ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

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
    Graphics_drawImage(&g_sContext, &DIRECTIONS, 0, 0);
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
}

int main(void)
{
    _hwInit();

    Graphics_drawImage(&g_sContext, &DIRECTIONS, 0, 0);

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

    int x_value;               // integer that contains the x value of the joystick
    int y_value;               // integer that contains the y value of the joystick
    int sign_x_value;          // 1, 0, -1 whether it's right, none, left direction
    int sign_y_value;          // 1, 0, -1 whether it's foreward, none, backward direction
    bool greater_module_x;     // greater value on left/right direction if true, greater value on foreward/backward direction if false


    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        x_value = (int) resultsBuffer[0];
        y_value = (int) resultsBuffer[1];

        drawDirections(x_value, y_value, sign_x_value, sign_y_value, greater_module_x);
    }
}

void drawDirections(int x, int y, int sign_x_value, int sign_y_value, bool greater_module_x){

    sign_x_value = 0;
    sign_y_value = 0;

    bool power_right = true;            // right if true, left if false
    int left_right_power = 0;           // number of power levels for left/right direction
    bool power_foreward = true;         // foreward if true, backward if false
    int foreward_backward_power = 0;    // number of power levels for foreward/backward direction
    int i;                              // index to draw the power levels

    //set parameters in order to choose the current direction and get te power levels
    if(x>9800){
        sign_x_value = 1;
        left_right_power = (int) (x-8200)/1400;
        power_right = true;
    } else if(x<7000){
        sign_x_value = -1;
        left_right_power = (int) (8200-x)/1400;
        power_right = false;
    }

    if(y>9800){
        sign_y_value = 1;
        foreward_backward_power = (int) (y-8200)/1400;
        power_foreward = true;
    } else if(y<7000){
        sign_y_value = -1;
        foreward_backward_power = (int) (8200-y)/1400;
        power_foreward = false;
    }

    // which is the most powerful direction: left/right or foreward/backward
    if(module_value(x-8200)>module_value(y-8200)){greater_module_x=true;} else {greater_module_x=false;}

    // left or right direction
    if(greater_module_x){
        if(sign_x_value == 1){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_RED, 76, 46);
            drawPower(left_right_power, foreward_backward_power, sign_x_value, sign_y_value, i);
        }
        if(sign_x_value == 0){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
        }
        if(sign_x_value == -1){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_RED, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
            drawPower(left_right_power, foreward_backward_power, sign_x_value, sign_y_value, i);
        }
    }

    // foreward or backward direction
    if(!greater_module_x){
        if(sign_y_value == 1){
            Graphics_drawImage(&g_sContext, &FOREWARD_RED, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
            drawPower(left_right_power, foreward_backward_power, sign_x_value, sign_y_value, i);
        }
        if(sign_y_value == 0){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_WHITE, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
        }
        if(sign_y_value == -1){
            Graphics_drawImage(&g_sContext, &FOREWARD_WHITE, 46, 8);
            Graphics_drawImage(&g_sContext, &BACKWARD_RED, 46, 76);
            Graphics_drawImage(&g_sContext, &LEFT_WHITE, 8, 46);
            Graphics_drawImage(&g_sContext, &RIGHT_WHITE, 76, 46);
            drawPower(left_right_power, foreward_backward_power, sign_x_value, sign_y_value, i);
        }
    }
}

int module_value(a){
    if(a<0) return -a;
    return a;
}

drawPower(int left_right_power, int foreward_backward_power, int sign_x_value, int sign_y_value, int i){

    if(sign_x_value == 1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 78+(POWER_OFFSET*i), 58);
        }
        drawHorizontalPower(sign_y_value, foreward_backward_power, i);
    }

    if(sign_x_value == -1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 47-(POWER_OFFSET*i), 58);
        }
        drawHorizontalPower(sign_y_value, foreward_backward_power, i);
    }

    if(sign_y_value == 1){
        for(i=0; i<foreward_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 47-(POWER_OFFSET*i));
        }
        drawVerticalPower(sign_x_value, left_right_power, i);
    }

    if(sign_y_value == -1){
        for(i=0; i<foreward_backward_power; i++){
            Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 78+(POWER_OFFSET+i));
        }
        drawVerticalPower(sign_x_value, left_right_power, i);
    }
}

void drawHorizontalPower(int sign_y_value, int foreward_backward_power, int i) {
        if(sign_y_value == 1){
            for(i=0; i<foreward_backward_power; i++){
                Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 47-(POWER_OFFSET*i));
            }
        } else if(sign_y_value == -1){
            for(i=0; i<foreward_backward_power; i++){
                Graphics_drawImage(&g_sContext, &HORIZONTAL_POWER, 58, 78+(POWER_OFFSET*i));
            }
        }
    }

void drawVerticalPower(int sign_x_value, int left_right_power, int i) {
    if(sign_x_value == 1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 78+(POWER_OFFSET*i), 58);
        }
    } else if(sign_x_value == -1){
        for(i=0; i<left_right_power; i++){
            Graphics_drawImage(&g_sContext, &VERTICAL_POWER, 47-(POWER_OFFSET*i), 58);
        }
    }
}
