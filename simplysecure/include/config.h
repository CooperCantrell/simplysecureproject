/*
 * File: Config.h
 * Author: Cooper Cantrell
 * Date 3-06-2025
 *
 * A config file for the simply project
 *
 */




// state machine config
#include <SimplyFSM.h>
#define SM_INIT {InitSimplyFSM}

// timers config
// keep this small my code is slow
#define TIMERPOSTSIZE 4
// sensors config MUST USE GPIO 5-9-----------
//captouch config 
#define CAPTOUCH_PIN GPIO_PIN_5
#define CAPTOUCH_Priority 2
//PING config
// ISR trigger pin
#define PING_ECHO GPIO_PIN_6
#define PING_TRIG GPIO_PIN_8
#define PING_Priority 2
// distance is in inches 
#define PING_CLOSE_THRESH 12
#define PING_CLOSE_TOL 5
// HAL  Config
#define HAL_PIN GPIO_PIN_7