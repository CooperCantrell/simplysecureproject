/*
 * File:   servo.h
 * Author: Caitlin 
 * Editor Cooper Cantrell
 * Created on 3/14/2025
 * Edited on 3/14/2025
 * EDIT 
 */

 #ifndef SERVO_H
 #define SERVO_H
/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include <stdint.h>
#include "timers.h"
#include "stm32f4xx_hal.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal_tim.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
/**
 * @Function ServoInit(void)
 * @param none
 * @return SUCCESS or ERROR
 * @brief INITS the servo module
 * @author CooperCantrell, 3/14/2025 
*/
char ServoInit(void);
/**
 * @Function ServoDuty(void)
 * @param Duty - 0 to 100 repersenting % Duty cycle
 * @return none
 * @brief sets the Duty cycle of the servo
 * @author CooperCantrell, 3/14/2025 
*/
void ServoDuty(uint8_t Duty);

 #endif