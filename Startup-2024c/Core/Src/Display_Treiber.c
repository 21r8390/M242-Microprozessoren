/*-----------------------------------------------------------------------
            D E F I N I T I O N    M O D U L E
-------------------------------------------------------------------------

PRODUCT:    Common Application (AP)

TITLE:      Treiber

PURPOSE:    Dieser Modul dient als Vorlage für neue Treiber.

------------------------------------------------------------------------*/

//  -------------------------------------------------
//  1. I N C L U D E S
//  -------------------------------------------------

#include "Display_Treiber.h"

//  -----------------------------------------------
//  3.     I N T E R N A L    D E F I N I T I O N S
//  -----------------------------------------------

// Where to display the dots
// Time format: Minutes . Seconds . Tenths
#define FORMAT_DECIMAL_POINT_M_S_F SEG_Driver_DP_2 | SEG_Driver_DP_4

// Time format: Minutes : Seconds
#define FORMAT_DECIMAL_POINT_M_S SEG_Driver_DP_3

//  -------------------------------------------
//  4.     I N T E R N A L    F U N C T I O N S
//  -------------------------------------------

// Converts the time to the format of the display
// The display can only display 4 digits
// The time is in tenths of a second
int ConvertTimeToDisplay(unsigned int tenths, int *decimalPoint)
{
    // Negative time is not supported by the display
    if (tenths <= 0)
    {
        *decimalPoint = SEG_Driver_DP_OFF;
        return 0;
    }

    int minutes = tenths / (10 * 60);   // Extract minutes
    int seconds = (tenths / 10) % 60;   // Extract seconds
    int tenths_of_second = tenths % 10; // Extract tenths of a second

    // Determine the format based on the number of minutes
    if (minutes < 10)
    {
        // Format: M:SS:F
        int displayValue = (minutes * 1000) + (seconds * 10) + tenths_of_second;
        *decimalPoint = FORMAT_DECIMAL_POINT_M_S_F;
        return displayValue;
    }
    else if (minutes < 100)
    {
        // Format: MM:SS
        int displayValue = (minutes * 100) + seconds;
        *decimalPoint = FORMAT_DECIMAL_POINT_M_S;
        return displayValue;
    }
    else
    {
        // Time is too big to display
        return 9999;
    }
}

// Writes the time to the SEG_Driver
void WriteTimeToDisplay(unsigned int time, int flashSpeed)
{
    // Convert the time to the format of the display
    int deciamelPoint = SEG_Driver_DP_OFF;
    int displayTime = ConvertTimeToDisplay(time, &deciamelPoint);

    // In order to write the display, theoretically a semaphore would have to be used.
    // But since the Seg_Driver was not written by me, I deliberately did without it.

    // Write the time to the display
    SEG_Driver_Write(displayTime, deciamelPoint, flashSpeed);
}

//  ---------------------------------------
//  5.     G L O B A L    F U N C T I O N S
//  ---------------------------------------

// Initializes the display and updates the display with the given value
void DisplayUpdateTask(void *argument)
{
    while (true)
    {
        // Update the display every 5ms
        SEG_Driver_Task_5ms();
        osDelay(5);
    }
}

// Updates the time of the display
void DisplayTime(unsigned int time)
{
    // Write the time to the display without flashing
    WriteTimeToDisplay(time, SEG_Driver_FLASH_OFF);
}

// Updates the time of the display with a flashing effect
void DisplayFlashTime(unsigned int time, enum DisplayFlashSpeed flashSpeed)
{
    // Convert the flash speed to the format of the SEG_Driver
    int flash;
    switch (flashSpeed)
    {
    case SLOW:
        flash = SEG_Driver_FLASH_SLOW;
        break;
    case FAST:
        flash = SEG_Driver_FLASH_FAST;
        break;
    default:
        flash = SEG_Driver_FLASH_OFF;
        break;
    }

    // Write the time to the display
    WriteTimeToDisplay(time, flash);
}
