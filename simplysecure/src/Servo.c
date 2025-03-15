/*
 * File:   servo.c
 * Author: Adam Korycki
 *
 * Created on October 5, 2023
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 #include <timers.h>
 #include <servo.h>
 
 // Boolean defines for TRUE, FALSE, SUCCESS and ERROR
 #ifndef FALSE
 #define FALSE ((int8_t) 0)
 #define TRUE ((int8_t) 1)
 #endif
 #ifndef ERROR
 #define ERROR ((int8_t) -1)
 #define SUCCESS ((int8_t) 1)
 #endif
 
 #define NUM_CHANNELS 6 // number of servo possible channels
 
 // User-level SERVO channels for inits/updating duty cycle etc...
 const SERVO SERVO_0 = {&htim1, TIM_CHANNEL_1, 0x1};
 const SERVO SERVO_1 = {&htim1, TIM_CHANNEL_2, 0x2};
 const SERVO SERVO_2 = {&htim1, TIM_CHANNEL_3, 0x4};
 const SERVO SERVO_3 = {&htim1, TIM_CHANNEL_4, 0x8};
 const SERVO SERVO_4 = {&htim4, TIM_CHANNEL_1, 0x10};
 const SERVO SERVO_5 = {&htim4, TIM_CHANNEL_3, 0x20};
 
 static unsigned int servo_freq = 1000; // [1 khz] default frequency 
 static uint32_t servo_duty_cycles[NUM_CHANNELS]; // to store the duty cycles of each channel
 static uint8_t servo_init_status = FALSE;
 static unsigned char servopinsAdded = 0x00;
 
 /**
  * @Function SERVO_Init(void)
  * @param None
  * @return SUCCESS or ERROR
  * @brief  Initializes the timer for the SERVO system and set is to the default frequency of 1 khz
  * @author Adam Korycki, 2023.10.05 */
 char SERVO_Init(void) {
     if (servo_init_status == FALSE) { // if SERVO module has not been inited
        //  // init TIM1
         TIM_ClockConfigTypeDef sClockSourceConfig = {0};
         TIM_MasterConfigTypeDef sMasterConfig = {0};
 
          uint32_t system_clock_freq = TIMERS_GetSystemClockFreq() / 1000000; // system clock freq in Mhz
        //  htim1.Instance = TIM1;
        //  htim1.Init.Prescaler = system_clock_freq - 1; // setting prescaler for 1 Mhz timer clock
        //  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
        //  htim1.Init.Period = 999; // deafault frequecy of 1 khz, changed by modifying ARRx register
        //  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        //  htim1.Init.RepetitionCounter = 0;
        //  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        //  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
        //  {
        //      return ERROR;
        //  }
        //  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        //  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
        //  {
        //      return ERROR;
        //  }
        //  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
        //  {
        //      return ERROR;
        //  }
        //  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        //  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        //  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
        //  {
        //      return ERROR;
        //  }
 
         // init TIM4
         htim4.Instance = TIM4;
         htim4.Init.Prescaler = (system_clock_freq) - 1; // setting prescaler for 1 Mhz timer clock
         htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
         htim4.Init.Period = 999; // deafault frequecy of 1 khz, changed by modifying ARRx register
         htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
         htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
         if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
         {
             return ERROR;
         }
         sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
         if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
         {
             return ERROR;
         }
         if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
         {
             return ERROR;
         }
         sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
         sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
         if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
         {
             return ERROR;
         }
         servo_init_status = TRUE;
     }
     return SUCCESS;
 }
 
 /**
  * @Function SERVO_AddPin(struct SERVO SERVO_x)
  * @param SERVO_x - SERVO channel to enable and configure
  * @return SUCCESS OR ERROR
  * @brief  Adds new pin to the SERVO system.
  * @author Adam Korycki, 2023.10.05 */
 char SERVO_AddPin(SERVO SERVO_x) {
     if (servo_init_status == FALSE) { // if servo module has not been initialized
         printf("ERROR: SERVO module has not yet been initialized!\r\n");
         return ERROR;
     }
     if ((servopinsAdded & SERVO_x.mask) != 0) { // if pin has already been added return ERROR
         printf("ERROR: this servo pin has already been added!\r\n");
         return ERROR;
     }
     TIM_OC_InitTypeDef sConfigOC = {0};
     sConfigOC.OCMode = TIM_OCMODE_PWM1;
     sConfigOC.Pulse = 0;
     sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
     sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
     sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
     sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
     sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
     if (HAL_TIM_PWM_ConfigChannel(SERVO_x.timer, &sConfigOC, SERVO_x.channel) != HAL_OK) {
         return ERROR;
     }
     servopinsAdded = servopinsAdded | SERVO_x.mask; // record added pin for book keeping
     HAL_TIM_MspPostInit(SERVO_x.timer);
     HAL_TIM_PWM_Start(SERVO_x.timer, SERVO_x.channel);
     //PWM_SetDutyCycle(SERVO_x, 50); // init to 50% DC
     return SUCCESS;
 }
 
 /**
  * @Function SERVO_SetFrequency(unsigned int NewFrequency)
  * @param NewFrequency - new frequency to set. must be between 50 hz and 50 khz
  * @return SUCCESS OR ERROR
  * @brief  Changes the frequency of the SERVO system.
  * @note  Behavior of SERVO channels during Frequency change is undocumented
  * @author Adam Korycki, 2023.10.05 */
 char SERVO_SetFrequency(unsigned int NewFrequency) {
     if (servo_init_status == FALSE) { // if servo module has not been initialized
         printf("ERROR: SERVO module has not yet been initialized!\r\n");
         return ERROR;
     }
     if ((NewFrequency < 100) || (NewFrequency > 100000)) { // if requested frequency is out of bounds
         return ERROR;
     }
     
     TIM4->ARR = (uint32_t)(1000000.0/NewFrequency) - 1; // set auto-reload registers (ARR) accordingly (1 Mhz timer)
     servo_freq = NewFrequency;
 
     // update to preserve duty cycle after frequency change
     unsigned char mask = 1;
     for (int i = 0; i < NUM_CHANNELS; i++) {
         if ((servopinsAdded & mask) != 0) { // if pin i has been added, update duty cycle with new frequency
             switch(i) {
                 case 0:
                     SERVO_SetDutyCycle(SERVO_0, servo_duty_cycles[i]);
                     break;
                 case 1:
                     SERVO_SetDutyCycle(SERVO_1, servo_duty_cycles[i]);
                     break;
                 case 2:
                     SERVO_SetDutyCycle(SERVO_2, servo_duty_cycles[i]);
                     break;
                 case 3:
                     SERVO_SetDutyCycle(SERVO_3, servo_duty_cycles[i]);
                     break;
                 case 4:
                     SERVO_SetDutyCycle(SERVO_4, servo_duty_cycles[i]);
                     break;
                 case 5:
                     SERVO_SetDutyCycle(SERVO_5, servo_duty_cycles[i]);
                     break;
             }
         }
         mask = mask << 1; // shift bitmask
     }
 
     return SUCCESS;
 }
 
 /**
  * @Function SERVO_GetFrequency(void)
  * @return Frequency of system in Hertz
  * @brief  gets the frequency of the SERVO system.
  * @author Adam Korycki, 2023.10.05 */
 unsigned int SERVO_GetFrequency(void) {
     return servo_freq;
 }
 
 /**
  * Function  SERVO_SetDutyCycle
  * @param SERVO_x - SERVO channel to start and set duty cyle
  * @param Duty - duty cycle for the channel (0-100)
  * @return SUCCESS or ERROR
  * @remark Enables the servo pin if not already enabled and sets the Duty Cycle for a Single Channel
  * @author Adam Korycki, 2023.10.05  */
 char SERVO_SetDutyCycle(SERVO SERVO_x, unsigned int Duty) {
     if (servo_init_status == FALSE) { // if servo module has not been initialized
         printf("ERROR: SERVO module has not yet been initialized!\r\n");
         return ERROR;
     }
     if ((servopinsAdded & SERVO_x.mask) == 0) { // if pin has not been added, add pin
         SERVO_AddPin(SERVO_x);
     }
     if ((Duty < 0) || (Duty > 100)) { // if requested duty cycle is out of bounds
         printf("ERROR: servo duty cycle must be between 0 and 100\r\n");
         return ERROR;
     }
     
     switch(SERVO_x.mask) { // set capture compare register (CCR) to correct value and save duty cycle value
         case 0x1: // SERVO_0
             TIM1->CCR1 = (uint32_t)((Duty/100.0)*(TIM1->ARR));
             servo_duty_cycles[0] = Duty;
             break;
         case 0x2: // SERVO_1
             TIM1->CCR2 = (uint32_t)((Duty/100.0)*(TIM1->ARR));
             servo_duty_cycles[1] = Duty;
             break;
         case 0x4: // SERVO_2
             TIM1->CCR3 = (uint32_t)((Duty/100.0)*(TIM1->ARR));
             servo_duty_cycles[2] = Duty;
             break;
         case 0x8: // SERVO_3
             TIM1->CCR4 = (uint32_t)((Duty/100.0)*(TIM1->ARR));
             servo_duty_cycles[3] = Duty;
             break;
         case 0x10: // SERVO_4
             TIM4->CCR1 = (uint32_t)((Duty/100.0)*(TIM1->ARR));
             servo_duty_cycles[4] = Duty;
             break;
         case 0x20: // SERVO_5
             TIM4->CCR3 = (uint32_t)((Duty/100.0)*(TIM1->ARR));
             servo_duty_cycles[5] = Duty;
             break;
     }
 
     return SUCCESS;
 }
 
 /**
  * Function: SERVO_Start
  * @param SERVO_x - SERVO channel to start
  * @return SUCCESS or ERROR
  * @remark Starts servo channel. Used to bring back up servo channles after SERVO_Stop()
  *         Must call SERVO_AddPin() or SERVO_SetDutyCycle() before using this function.
  * @author Adam Korycki, 2023.11.27  */
 char SERVO_Start(SERVO SERVO_x) {
     if (servo_init_status == FALSE) { // if servo module has not been initialized
         printf("ERROR: SERVO module has not yet been initialized!\r\n");
         return ERROR;
     }
     if ((servopinsAdded & SERVO_x.mask) == 0) { // if pin has not been added
         printf("ERROR: SERVO pin has not been added!\r\n");
         return ERROR;
     }
     // start servo channel
     HAL_TIM_PWM_Start(SERVO_x.timer, SERVO_x.channel);
     return SUCCESS;
 }
 
 /**
  * Function: SERVO_Stop
  * @param None
  * @return SUCCESS or ERROR
  * @remark Stops servo channel. Use SERVO_Start() to start channel again
  *         Must call SERVO_AddPin() or SERVO_SetDutyCycle() before using this function.
  * @author Adam Korycki, 2023.11.27  */
 char SERVO_Stop(SERVO SERVO_x) {
     if (servo_init_status == FALSE) { // if servo module has not been initialized
         printf("ERROR: SERVO module has not yet been initialized!\r\n");
         return ERROR;
     }
     if ((servopinsAdded & SERVO_x.mask) == 0) { // if pin has not been added
         printf("ERROR: SERVO pin has not been added!\r\n");
         return ERROR;
     }
     // start servo channel
     HAL_TIM_PWM_Stop(SERVO_x.timer, SERVO_x.channel);
     return SUCCESS;
 }
 
 /**
  * Function: SERVO_End
  * @param None
  * @return SUCCESS or ERROR
  * @remark Deinitializes the SERVO system
  * @author Adam Korycki, 2023.10.05  */
 char SERVO_End(void) {
     if (servo_init_status == FALSE) { // if servo module has not been initialized
         printf("ERROR: SERVO module has not yet been initialized!\r\n");
         return ERROR;
     }
     // stop all servo channels
     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
     HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
     HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
 
     //deinitialize timer peripherals
     HAL_TIM_PWM_DeInit(&htim1);
     HAL_TIM_PWM_DeInit(&htim4);
     HAL_TIM_Base_DeInit(&htim1);
     HAL_TIM_Base_DeInit(&htim4);
     return SUCCESS;
 }
 
 // SERVO TEST HARNESS
 //#define SERVO_TEST
 #ifdef SERVO_TEST
 // SUCCESS - 
  // SERVO_0 = 10 DC
  // SERVO_1 = 20 DC
  // SERVO_2 = 30 DC
  // SERVO_3 = 50 DC
  // SERVO_4 = 75 DC
  // SERVO_5 = 100 DC
  // ALL CHANNELS @ 5 khz
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <Board.h>
 #include <timers.h>
 #include <servo.h>
 
 int main(void) {
     BOARD_Init();
     TIMER_Init();
     if (SERVO_Init() == SUCCESS) {
         printf("servo initialized\r\n");
 
         // add all pins and set duty cycles
         if (SERVO_SetDutyCycle(SERVO_0, 10) == ERROR) {
             printf("servo set duty cycle error\r\n");
             return -1;
         }
         if (SERVO_SetDutyCycle(SERVO_1, 20) == ERROR) {
             printf("servo set duty cycle error\r\n");
             return -1;
         }
         if (SERVO_SetDutyCycle(SERVO_2, 30) == ERROR) {
             printf("servo set duty cycle error\r\n");
             return -1;
         }
         if (SERVO_SetDutyCycle(SERVO_3, 50) == ERROR) {
             printf("servo set duty cycle error\r\n");
             return -1;
         }
         if (SERVO_SetDutyCycle(SERVO_4, 75) == ERROR) {
             printf("servo set duty cycle error\r\n");
             return -1;
         }
         if (SERVO_SetDutyCycle(SERVO_5, 100) == ERROR) {
             printf("servo set duty cycle error\r\n");
             return -1;
         }
         SERVO_SetFrequency(5000);
     }
 
     while (TRUE);
 }
 
 #endif

// /*******************************************************************************
//  * Private INCLUDES                                                           *
//  ******************************************************************************/
// #include "Servo.h"
// #include "config.h"

// #define FIFTYHZ 20000
// /*******************************************************************************
//  * Private Variables                                                           *
//  ******************************************************************************/

// TIM_HandleTypeDef htim5;

// float PWMDC = .5;
// uint8_t CurrentState = 0;

// /*******************************************************************************
//  * Private FUNCTIONS                                                          *
//  ******************************************************************************/
// void TIM5_IRQHandler(void)
// {
//     if (__HAL_TIM_GET_IT_SOURCE(&htim5, TIM_IT_UPDATE) != RESET)
//     {
//         __HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE); // clear interrupt flag

//         // state machine of your design
//         switch (CurrentState)
//         {
//         case 0:
//         // Timer is 10k Hz
//             CurrentState = 1;
//             HAL_GPIO_WritePin(GPIOB, SERVO_PIN, GPIO_PIN_SET);
//             TIM5->ARR = FIFTYHZ *PWMDC;
//             break;

//         case 1:
//             CurrentState = 0;
//             HAL_GPIO_WritePin(GPIOB, SERVO_PIN, GPIO_PIN_RESET);
//             TIM5->ARR = FIFTYHZ *(1-PWMDC);

//             break;
//         default:
//             break;
//         }
//     }
// }

// /*******************************************************************************
//  * PUBLIC FUNCTIONS                                                           *
//  ******************************************************************************/
// /**
//  * @Function ServoInit(void)
//  * @param none
//  * @return ERROR or SUCCESSES
//  * @brief inits all of the servo controller module
//  * @author Caitlin Bonesio
// */
// char ServoInit(void){
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = SERVO_PIN;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//     TIM_MasterConfigTypeDef sMasterConfig = {0};
//     htim5.Instance = TIM5;
//     htim5.Init.Prescaler = 83; // divide by 1 prescaler (84-1) = 1 Mhz tick
//     htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
//     htim5.Init.Period = FIFTYHZ; // MUST CHANGE. number of clock cycles between interrupts
//     htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//     htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//     if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
//     {
//         return ERROR;
//     }
//     sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//     if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
//     {
//         return ERROR;
//     }
//     sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//     sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//     if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
//     {
//         return ERROR;
//     }
//     HAL_TIM_Base_Start_IT(&htim5); // start interrupt
//     return SUCCESS;
// }

// /**
//  * @Function ServoReadDC(void)
//  * @param none
//  * @return float between 0 and 1 representing the current duty cycle
//  * @brief read the current duty cycle
//  * @author Caitlin Bonesio
// */
// float ServoReadDC(void){
//     return PWMDC;
// }

// /**
//  * @Function ServoSetDC(void)
//  * @param a float between 0 and 1 representing the next duty cycle
//  * @return none
//  * @brief sets the current duty cycle
//  * @author Caitlin Bonesio
// */
// void ServoSetDC(float newDC){
//     PWMDC = newDC;
// }
