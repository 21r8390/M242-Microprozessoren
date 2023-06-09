/*-----------------------------------------------------------------------
            D E F I N I T I O N    M O D U L E
-------------------------------------------------------------------------

PRODUCT:    Common Application (AP)

TITLE:      Buzzer_Treiber

PURPOSE:    Dieser Treiber steuert den Buzzer an.

------------------------------------------------------------------------*/

#ifndef _BUZZER_TREIBER
#define _BUZZER_TREIBER

//  -------------------------------------------------
//  1. I N C L U D E S
//  -------------------------------------------------

#include "main.h"
#include "cmsis_os.h"
#include <stdbool.h>

//  -------------------------------------------
//  2.     G L O B A L    D E F I N I T I O N S
//  -------------------------------------------

// Semaphore controlling the access to the buzzer
extern osSemaphoreId_t buzzerSemaphoreHandleId;

//  ---------------------------------------
//  3.     G L O B A L    C O N S T A N T S
//  ---------------------------------------

//  ---------------------------------------
//  5.     G L O B A L    F U N C T I O N S
//  ---------------------------------------

// Initializes the buzzer and updates the buzzer task
// This task will control the state of buzzer in the background
void BuzzerTask(void *argument);

// Beep the buzzer for a given length in ms
void BuzzerBeep(int length);

#endif
