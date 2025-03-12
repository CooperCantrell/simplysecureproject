/**
 * File: main.c
 * Author: Cooper Cantrell
 * Created 3/6/2025
 *
 * The main that will run on the hardware of our project,
 * this project is designed for interrupt driven development
 * so the main function is quite barren
 *
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include <stdio.h>
#include <Board.h>
#include <timers.h>
#include <TODOQueue.h>
#include <Sensors.h>
#include <SimplyFSM.h>
#include <leds.h>
//#include <PING.h>
#include <TimerPost.h>
int main(void)
{
    
    // Your hardware  function calls go here ---------------------------------------
    BOARD_Init();
    TIMER_Init();
    SensorInit();
    LEDS_Init();
    TimerPostInit();
    printf("Lock Firmware for the lock is running\r\n");
    // Running as normal -----------------------------------------------------------
    RunQueue();
    // If we ever reach here something went very wrong ----------------------------
    printf("--ERROR--\n--ERROR--\n--ERROR--\nSomething went very wrong if you see this [state machine returned ERROR]\n--ERROR--\n--ERROR--\n--ERROR--\r\n");
    return -1;
}
