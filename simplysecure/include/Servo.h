/*
 * File:   servo.h
 * Author: Adam Korycki
 *
 * Created on October 5, 2023
 */

 #ifndef SERVO_H
 #define	SERVO_H
 
 #include <stdint.h>
 #include "stm32f4xx_hal.h"
 #include "stm32f411xe.h"
 #include "stm32f4xx_hal_tim.h"
 
 void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
 
 TIM_HandleTypeDef htim1;
 TIM_HandleTypeDef htim4;
 
 // servo channel struct with timer and channel attributes + bit mask for book keeping
 typedef struct SERVO {
     TIM_HandleTypeDef* timer;
     unsigned int channel;
     unsigned char mask;
 } SERVO;  
 
 // user-level SERVO channels
//  extern const SERVO SERVO_0;
//  extern const SERVO SERVO_1;
//  extern const SERVO SERVO_2;
//  extern const SERVO SERVO_3;
 extern const SERVO SERVO_4;
//  extern const SERVO SERVO_5;
 
 /**
  * @Function SERVO_Init(void)
  * @param None
  * @return SUCCESS or ERROR
  * @brief  Initializes the timer for the SERVO system and set to the default frequency of 1 khz
  * @note  None.
  * @author Adam Korycki, 2023.10.05 */
 char SERVO_Init(void);
 
 /**
  * @Function SERVO_SetFrequency(unsigned int NewFrequency)
  * @param NewFrequency - new frequency to set. must be between 100 hz and 100 khz
  * @return SUCCESS OR ERROR
  * @brief  Changes the frequency of the SERVO system.
  * @note  Behavior of SERVO channels during Frequency change is undocumented
  * @author Adam Korycki, 2023.10.05 */
 char SERVO_SetFrequency(unsigned int NewFrequency);
 
 /**
  * @Function SERVO_GetFrequency(void)
  * @return Frequency of system in Hertz
  * @brief  gets the frequency of the SERVO system.
  * @author Adam Korycki, 2023.10.05 */
 unsigned int SERVO_GetFrequency(void);
 
 /**
  * @Function SERVO_AddPin(SERVO SERVO_x)
  * @param SERVO_x - SERVO channel to enable and configure
  * @return SUCCESS OR ERROR
  * @brief  Adds new pin to the SERVO system. If any pin is already active it errors
  * @author Adam Korycki, 2023.10.05 */
 char SERVO_AddPin(SERVO SERVO_x);
 
 /**
  * Function  SERVO_SetDutyCycle(SERVO SERVO_x, unsigned int Duty)
  * @param SERVO_x - SERVO channel to start and set duty cyle
  * @param Duty - duty cycle for the channel (0-100)
  * @return SUCCESS or ERROR
  * @remark Enables the servo pin if not already enabled and sets the Duty Cycle for a Single Channel
  * @author Adam Korycki, 2023.10.05  */
 char SERVO_SetDutyCycle(SERVO SERVO_x, unsigned int Duty);
 
 /**
  * Function: SERVO_Start
  * @param SERVO_x - SERVO channel to start
  * @return SUCCESS or ERROR
  * @remark Starts servo channel. Used to bring back up servo channles after SERVO_Stop()
  *         Must call SERVO_AddPin() or SERVO_SetDutyCycle() before using this function.
  * @author Adam Korycki, 2023.11.27  */
 char SERVO_Start(SERVO SERVO_x);
 
 /**
  * Function: SERVO_Stop
  * @param None
  * @return SUCCESS or ERROR
  * @remark Stops servo channel. Use SERVO_Start() to start channel again
  *         Must call SERVO_AddPin() or SERVO_SetDutyCycle() before using this function.
  * @author Adam Korycki, 2023.11.27  */
 char SERVO_Stop(SERVO SERVO_x);
 
 /**
  * Function: SERVO_End
  * @param None
  * @return SUCCESS or ERROR
  * @remark Disables the SERVO sub-system and releases all pins.
  * @author Adam Korycki, 2023.10.05  */
 char SERVO_End(void);
 
 
 #endif
 
// /* 
//  * File:   Servo.h
//  * Author: Caitlin Bonesio
//  * Editor: --
//  * Created on 3/8/2025
//  * Controls Timer 4 to output a PWM frequency that is not dependent on the SERVO module
//  */
// #ifndef SERVO_H  
// #define SERVO_H 
// /*******************************************************************************
//  * PUBLIC #INCLUDES                                                            *
//  ******************************************************************************/
// #include "timers.h"
// #include "stm32f4xx_hal.h"
// #include "stm32f411xe.h"
// #include "stm32f4xx_hal_tim.h"

// /*******************************************************************************
//  * PUBLIC #DEFINES                                                             *
//  ******************************************************************************/


// /*******************************************************************************
//  * PUBLIC TYPEDEFS                                                             *
//  ******************************************************************************/


// /*******************************************************************************
//  * PUBLIC FUNCTION PROTOTYPES                                                  *
//  ******************************************************************************/
// /**
//  * @Function ServoInit(void)
//  * @param none
//  * @return ERROR or SUCCESSES
//  * @brief inits all of the servo controller module
//  * @author Caitlin Bonesio
// */
// char ServoInit(void);

// /**
//  * @Function ServoReadDC(void)
//  * @param none
//  * @return float between 0 and 1 representing the current duty cycle
//  * @brief read the current duty cycle
//  * @author Caitlin Bonesio
// */
// float ServoReadDC(void);

// /**
//  * @Function ServoSetDC(void)
//  * @param a float between 0 and 1 representing the next duty cycle
//  * @return none
//  * @brief sets the current duty cycle
//  * @author Caitlin Bonesio
// */
// void ServoSetDC(float newDC);


// #endif 