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
#include "UnlockFSM.h"
#define SM_INIT {InitSimplyFSM, InitUnlockFSM}

// timers config
// keep this small my code is slow
#define TIMERPOSTSIZE 64
#define FORBIDDEN_ID -1
// sensors config MUST USE GPIO 5-9-----------
//captouch config 
// NOTE GPIOB
#define CAPTOUCH_PIN GPIO_PIN_5
#define CAPTOUCH_Priority 2
//PING config
// ISR trigger pin
#define PING_ECHO GPIO_PIN_9
#define PING_TRIG GPIO_PIN_8
#define PING_Priority 2
// distance is in inches 
#define PING_CLOSE_THRESH 24
#define PING_CLOSE_TOL 6
#define PING_MISS_THRESH 470
// HAL  Config
#define HAL_PIN GPIO_PIN_7
#define HAL_Priority 0
#define HAL_DEBOUNCE_WAIT 10
#define HAL_DEBOUNCE_ID 9
//Servo Config
//#define SERVO_PIN GPIO_PIN_12
#define SERVO_Priority 2
// temp
#define HEARTID 64
// Speakers
#include <ADC.h>
#include <pwm.h>
#define POTPIN ADC_0
#define SPEAKERPIN PWM_0
//Servos
#define SERVO_FREQ 50
#define SERVO_LOCKED 80
#define SERVO_UNLOCKED 20
//note this is GPIOB, pin 61
#define SERVO_PIN GPIO_PIN_10