#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

extern const Graphics_Image MarioBrosTile8BPP_UNCOMP;
extern const Graphics_Image Mario8BPP_UNCOMP;

#define TILE_SPRITE     MarioBrosTile8BPP_UNCOMP
#define MARIO_SPRITE    Mario8BPP_UNCOMP

#define SPRITE_OFFSET   16              // Every image is 16x16
#define MARIO_Y_POS     128-SPRITE_OFFSET*2
#define MOVE_INCREMENT  4

// Initialization function for graphics
void initializeGraphics(Graphics_Context* g_sContext_p) {
    // Initialize the LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize context
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    // Set colors and fonts
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_SLATE_BLUE);
    Graphics_setFont(g_sContext_p, &g_sFontCmss12);

    // Clear the screen
    Graphics_clearDisplay(g_sContext_p);
}

// Function to restart timer
void restartTimer() {
    Timer32_setCount(TIMER32_0_BASE, 3000000/4); // 0.25 seconds
    Timer32_startTimer(TIMER32_0_BASE, true);
}

// Function to check if timer is expired
bool isTimerExpired() {
    return (Timer32_getValue(TIMER32_0_BASE) == 0);
}

int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Initializations
    Graphics_Context g_sContext; // Create context
    initializeGraphics(&g_sContext);

    // Draw initial graphics
    int i;
    for(i = 0; i < (128/SPRITE_OFFSET); i++) { // Draw the row of tiles
        Graphics_drawImage(&g_sContext, &TILE_SPRITE, i*SPRITE_OFFSET, 128-SPRITE_OFFSET);
    }
    Graphics_drawImage(&g_sContext, &MARIO_SPRITE, 0, MARIO_Y_POS);

    // Initialize and start a one-shot timer
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    restartTimer();

    // Start demo
    unsigned x = 0, y = MARIO_Y_POS;
    unsigned loop = 0;
    while(loop < (128/MOVE_INCREMENT)) { // Draw until Mario goes off screen
        if(isTimerExpired()) {
            // Create rectangle
            Graphics_Rectangle eraseBox;
            eraseBox.xMin = x;
            eraseBox.yMin = y;
            eraseBox.xMax = eraseBox.xMin + SPRITE_OFFSET - 1;
            eraseBox.yMax = eraseBox.yMin + SPRITE_OFFSET - 1;

            // Erase Mario
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_SLATE_BLUE);
            Graphics_fillRectangle(&g_sContext, &eraseBox);
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);

            // Update position -- y remains constant!
            x += MOVE_INCREMENT;

            // Redraw Mario
            Graphics_drawImage(&g_sContext, &MARIO_SPRITE, x, y);

            loop++;
            restartTimer();
        }
    }

    while(1);
}
