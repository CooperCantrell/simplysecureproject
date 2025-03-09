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
#define SM_INIT {InitTemplateFSM}

// timers config
// keep this small my code is slow
#define TIMERPOSTSIZE 4
// sensors config MUST USE GPIO 5-9-----------
//captouch config 
#define CAPTOUCH_PIN GPIO_PIN_5
#define CAPTOUCH_Priority 2

