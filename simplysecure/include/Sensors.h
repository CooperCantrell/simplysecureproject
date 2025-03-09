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
 * @return none
 * @brief inits all of the sensors together so the ISR works better
 * @author Cooper Cantrell, DATE 
 * @edited EDITORNAME, DATE - only if applies
*/
void SensorInit(void);


#endif 