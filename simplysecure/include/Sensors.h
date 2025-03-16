/* 
 * File:   Sensors.h
 * Author: Cooper Cantrell
 * Editor: --
 * Created on 3/8/2025
 * Needed as our sensors share an ISR GPIO5-9
 */
#ifndef SENSOR_H  
#define SENSOR_H 
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
 * @Function SensorInit(void)
 * @param none
 * @return ERROR or SUCCESSES
 * @brief inits all of the sensors together so the ISR works better
 * @author Cooper Cantrell, DATE 
 * @edited EDITORNAME, DATE - only if applies
*/
char SensorInit(void);
/**
 * @Function SERVO_SetDutyCycle(uint16_t duty)
 * @param new duty cycle, must be between 1-99
 * @return none
 * @brief inits all of the sensors together so the ISR works better
 * @author Caitlin Bonesio
 * @edited EDITORNAME, DATE - only if applies
*/
void SERVO_SetDutyCycle(uint16_t duty);


#endif 