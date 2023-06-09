/*-----------------------------------------------------------------------
            D E F I N I T I O N    M O D U L E
-------------------------------------------------------------------------

PRODUCT:    Common Application (AP)

TITLE:      Timer

PURPOSE:    Diese Datei ist für die Event-Steuerung des Timers zuständig.

------------------------------------------------------------------------*/

//  -------------------------------------------------
//  1. I N C L U D E S
//  -------------------------------------------------

#include "Timer.h"

//  -----------------------------------------------
//  3.     I N T E R N A L    D E F I N I T I O N S
//  -----------------------------------------------

// The current state if the timer is configuring
static bool isConfiguringTimer = false;
// The current state of the timer
static bool isTimerRunning = false;
// The current state if the time is up
static bool isTimeUp = false;

// The current time of the timer in tenths of a second
static unsigned int currentTime = 0;

// Callback function when the timer is up
static void (*timerUpCallback)();

//  -------------------------------------------
//  4.     I N T E R N A L    F U N C T I O N S
//  -------------------------------------------

//  ---------------------------------------
//  5.     G L O B A L    F U N C T I O N S
//  ---------------------------------------

// The background task to initialize the timer
// This will create a task for the timer
void TimerTask(void *argument)
{
    while (true)
    {
        // Only increase the time if the timer is running
        if (IsTimerRunning() && !IsTimerPaused() && !IsConfiguringTimer() && !IsTimeUp())
        {
            // Check if the time is up
            if (currentTime > 1)
            {
                // Decrease the time by one tenth of a second
                currentTime--;
            }
            else
            {
                // The time is up and the timer is not running anymore and the time is not negative
                currentTime = 0;
                isTimeUp = true;

                // Call the function to notify the timer is up
                (*timerUpCallback)();
            }
        }

        // Increase the time every tenth of a second
        osDelay(100);
    }
}

// Sets the callback function when the timer is up
void SetTimerUpCallback(void (*callback)())
{
    timerUpCallback = callback;
}

// Increments the configured time
void IncrementTime()
{
    // Only increase the time if the timer is not running
    if (IsConfiguringTimer())
    {
        // Increase the time by one second
        currentTime += 10;
    }
}

// Decrements the configured time
void DecrementTime()
{
    // Only decrease the time if the timer is configuring
    if (IsConfiguringTimer())
    {
        if (currentTime > 10)
        {
            // Decrease the time by one second
            currentTime -= 10;
        }
        else
        {
            // The time cannot be negative
            currentTime = 0;
        }
    }
}

// Start the timer
void StartTimer()
{
    // Timer is not configuring anymore
    isConfiguringTimer = false;

    // Start the timer
    isTimeUp = false;
    isTimerRunning = true;
}

// Pauses the timer
void PauseTimer()
{
    // Only pause the timer if the timer is running
    if (IsTimerRunning())
    {
        // Pause the timer
        isTimerRunning = false;
    }
}

// Resumes the timer
void ResumeTimer()
{
    // Only resume the timer if the timer is running and not up
    if (!IsConfiguringTimer())
    {
        // Resume the timer
        isTimerRunning = true;
    }
}

// Stops the timer
void ResetTimer()
{
    // Stop the timer
    isTimerRunning = false;

    // Reset the pause
    isConfiguringTimer = true;

    // Reset the time up
    isTimeUp = false;

    // Reset the time
    currentTime = 0;
}

// Returns the current time
unsigned int GetTimerTime()
{
    return currentTime;
}

// Returns true if the timer is configuring
bool IsConfiguringTimer()
{
    return isConfiguringTimer;
}

// Returns true if the timer is running
bool IsTimerRunning()
{
    return isTimerRunning;
}

// Returns true if the timer is paused
bool IsTimerPaused()
{
    return !isTimerRunning && !isConfiguringTimer;
}

// Returns true if the time is up
bool IsTimeUp()
{
    return isTimeUp;
}
