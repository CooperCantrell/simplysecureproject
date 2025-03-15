/* 
 * File:   Servo.h
 * Author: Caitlin Bonesio
 * Editor: --
 * Created on 3/8/2025
 * Controls Timer 4 to output a PWM frequency that is not dependent on the PWM module
 */
#ifndef SERVO_H  
#define SERVO_H 
/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
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
 * @return ERROR or SUCCESSES
 * @brief inits all of the servo controller module
 * @author Caitlin Bonesio
*/
void ServoInit(void);

/**
 * @Function ServoReadDC(void)
 * @param none
 * @return float between 0 and 1 representing the current duty cycle
 * @brief read the current duty cycle
 * @author Caitlin Bonesio
*/
float ServoReadDC(void);

/**
 * @Function ServoSetDC(void)
 * @param a float between 0 and 1 representing the next duty cycle
 * @return none
 * @brief sets the current duty cycle
 * @author Caitlin Bonesio
*/
void ServoSetDC(float newDC);


#endif 