// /*
//  * File:   servo.h
//  * Author: Caitlin 
//  * Editor Cooper Cantrell
//  * Created on 3/14/2025
//  * Edited on 3/14/2025
//  * EDIT 
//  */

// /*******************************************************************************
//  * MODULE #INCLUDE                                                             *
//  ******************************************************************************/
// #include <Servos.h>
// #include <config.h>
// #include <leds.h>
// /*******************************************************************************
//  * PRIVATE #DEFINES                                                            *
//  ******************************************************************************/


// /*******************************************************************************
//  * PRIVATE TYPEDEFS                                                            *
//  ******************************************************************************/
// #define SERVORFREQUENCY 50
// /*******************************************************************************
//  * PRIVATE VARIABLES                                                           *
//  ******************************************************************************/
// TIM_HandleTypeDef htim3;
// static uint8_t init_status = FALSE;
// static uint8_t DUTY;
// enum States
// {
//     OFF,
//     ON,
// };
// enum States CurrentState;
// /*******************************************************************************
//  * PRIVATE FUNCTIONS/CLASSES                                                   *
//  ******************************************************************************/
// #include <stdio.h>
// void TIM3_IRQHandler(void)
// {
//     if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET)
//     {
//         __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag
//         printf("SERV\n");
//         switch (CurrentState)
//         {
//         case OFF:
//             CurrentState = ON;
//             HAL_GPIO_WritePin(GPIOA,SERVO_PIN,GPIO_PIN_SET);
//             set_leds(1);
//             TIM3->ARR = SERVORFREQUENCY *10 *DUTY/100;
//             break;
//         case ON:
//             CurrentState = OFF;
//             HAL_GPIO_WritePin(GPIOA,SERVO_PIN,GPIO_PIN_RESET);
//             TIM3->ARR =  SERVORFREQUENCY *10 * (100 - DUTY)/100;
//             set_leds(0);
//             break;
//         default:
//             break;
//         }
//     }
// }
// // void TIM3_IRQHandler(void)
// // {
// //     if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET)
// //     {
// //         __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag

// //         // state machine of your design
// //         switch (CurrentState)
// //         {
// //         case Waiting:
// //         // Timer is 10k Hz
// //             CurrentState = Trigger;
// //             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
// //             TIM3->ARR = 1;
// //             break;

// //         case Trigger:
// //             CurrentState = Waiting;
// //             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
// //             TIM3->ARR = 1000;

// //             break;
// //         default:
// //             break;
// //         }
// //     }
// // }
// /*******************************************************************************
//  * PUBLIC FUNCTIONS FUNCTIONS                                                  *
//  ******************************************************************************/

// // define functions in the .h file

// /**
//  * @Function ServoInit(void)
//  * @param none
//  * @return SUCCESS or ERROR
//  * @brief INITS the servo module
//  * @author CooperCantrell, 3/14/2025 
// */
// char ServoInit(void){
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = SERVO_PIN;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
//     CurrentState = OFF;
//     DUTY = 50;
//     // htim3.Instance->SR = 0;//clear all interrupt flags
//     // htim3.Instance->CR1 = 0;//clear previous settings
//     // htim3.Instance->CR2 = 0;//clear previous settings
//     // htim3.Instance->SMCR = 0;//clear previous slave settings settings
//     // htim3.Instance->DIER = 0;//clear previous interrupt settings
//     // htim3.Instance->CCER = 0;//disable OC and IC modules compltely
//     // htim3.Instance->ARR = SERVORFREQUENCY*10*DUTY/100;
//     // uint32_t initParam = 0;
//     // initParam |= TIM_CR1_URS;//Only counter overflows generate and event
//     // initParam |= TIM_CR1_CEN;//will enable the counter
//     // htim3.Instance->CR1 = initParam;//note tim3 does not have a cr2 register, do not believe HAL
//     // htim3.Instance->PSC = (uint16_t)(HAL_RCC_GetSysClockFreq()/(SERVORFREQUENCY+1)/1000);//set prescaler to set the frequency to SERVOFREQUENCY*1000
//     // htim3.Instance->DIER |= TIM_DIER_UIE;//enable interrupt
//     // htim3.Instance->EGR |= TIM_EGR_UG;//force hardware to reset the count, prescaler counter, and update all registers



//     if (init_status == FALSE) { // if TIM2 module has not been initialized
//         TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//         TIM_MasterConfigTypeDef sMasterConfig = {0};

//         uint32_t system_clock_freq = TIMERS_GetSystemClockFreq() / 1000000; // system clock freq in Mhz
//         htim2.Instance = TIM2;
//         htim2.Init.Prescaler = system_clock_freq - 1; // setting prescaler for 1 Mhz timer clock
//         htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
//         htim2.Init.Period = 999; //trigger interrupt every 1ms
//         htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//         htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//         if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
//         {
//             return ERROR;
//         }
//         sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//         if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
//         {
//             return ERROR;
//         }
//         sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//         sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//         if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
//         {
//             return ERROR;
//         }

//         HAL_TIM_Base_Start_IT(&htim2); // start interrupt
//         init_status = TRUE;
//     }
//     return SUCCESS;
//     // TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//     // TIM_MasterConfigTypeDef sMasterConfig = {0};
//     // htim3.Instance = TIM3;
//     // htim3.Init.Prescaler = ((83));//*1000000)/(SERVO_FREQ*100); // 50hz/100
//     // htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//     // htim3.Init.Period = 100; // MUST CHANGE. number of clock cycles between interrupts
//     // htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//     // htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//     // if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
//     // {
//     //     return ERROR;
//     // }
//     // sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//     // if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
//     // {
//     //     return ERROR;
//     // }
//     // sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//     // sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//     // if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
//     // {
//     //     return ERROR;
//     // }
//     //HAL_TIM_Base_Start_IT(&htim3); // start interrupt

//     //DUTY = 50;
//     return SUCCESS;
// }
// /**
//  * @Function ServoDuty(void)
//  * @param Duty - 0 to 100 repersenting % Duty cycle
//  * @return none
//  * @brief sets the Duty cycle of the servo
//  * @author CooperCantrell, 3/14/2025 
// */
// void ServoDuty(uint8_t Duty){
//     DUTY = Duty;
// }


