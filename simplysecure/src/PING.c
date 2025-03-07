// /**
//  * @file    PING.c
//  * @brief   Ultrasonic Ping Sensor sensing module
//  * @author  Cooper Cantrell
//  * @date    1/25/2025
//  * @detail  This module uses the timer interrupt and extrenal trigger interrupt (EXTI) to drive the HC-SR04
//  *          ultrasonic ranging sensor.
//  *          You will use GPIO pin PB8 (PWM_5) to toggle the 10 uS trigger pulse and GPIO pin PB5 (ENC_B) as an
//  *          external interrupt which is used to record the state of the echo pin (PB5). A timer-generated (TIM3)
//  *          interrupt will be used to facillitate a state machine of your design. This interrupt is configured with
//  *          a 1:84 prescaler meaning it ticks at the frequency of the 1 Mhz. The period between interrupts
//  *          is changed by modifying the ARR register. Please see the appendix on Timer interrupts. Lastly, you will
//  *          use the timers.h library to record timing information and calculate pulse durations.
//  */

// /*******************************************************************************
//  * MODULE #INCLUDE                                                             *
//  ******************************************************************************/
// #include <PING.h>
// #include <timers.h>
// #include <pwm.h>
// #include <math.h>
// /*******************************************************************************
//  * PRIVATE #DEFINES                                                            *
//  ******************************************************************************/
// //#define PING_TEST
// #ifdef PING_TEST
// #include <stdio.h>
// #endif
// #define X_COEF 0.006735130021308
// #define BIAS 0.239378770600926
// #define IN2CM 2.54
// /*******************************************************************************
//  * PRIVATE VARIABLES                                                           *
//  ******************************************************************************/
// // states handles both the simple timer state machine and the tracking of
// // the riseing and falling edges of the trigger handler
// enum States
// {
//     Waiting,
//     Trigger,
// };
// enum States CurrentState;
// uint32_t TimeOfFlight;
// uint32_t REdgeRead; // Riseing edge read
// uint32_t FEdgeRead; // Falling
// /*******************************************************************************
//  * PRIVATE FUNCTIONS                                                           *
//  ******************************************************************************/
// // external interrupt ISR for pin PB5 (BLUE INTERUPT WIRE)
// void EXTI9_5_IRQHandler(void)
// {
//     // EXTI line interrupt detected
//     if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET)
//     {
//         __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag

//         // anything that needs to happen when PB5 (ENC_B) changes state
//         if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)){ // trigger on rise edge
//             REdgeRead = TIMERS_GetMicroSeconds();
//         }
//         else{ // fall edge
//             FEdgeRead = TIMERS_GetMicroSeconds();
//             TimeOfFlight = FEdgeRead-REdgeRead;
//         }
//     }
// }

// // TIM3 ISR
// void TIM3_IRQHandler(void)
// {
//     if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET)
//     {
//         __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag

//         // state machine of your design
//         switch (CurrentState)
//         {
//         case Waiting:
//             CurrentState = Trigger;
//             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
//             // set timer to 10us
//             // timer ticks 1 per us
//             TIM3->ARR = 10;
//             break;

//         case Trigger:
//             CurrentState = Waiting;
//             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
//             // set timer to 60ms
//             // timer ticks 1 per us
//             TIM3->ARR = 60000;
//             break;
//         default:
//             break;
//         }
//     }
// }

// /**
//  * @function    TimeFlight2cm(uint32_t time)
//  * @brief       returns the distane in inches given the time of flight as input 
//  * useing the polynomial found using lsqr in matlab
//  * @return      double the time distance in cm
//  */
// double TimeFlight2cm(uint32_t time){
//     // this was for inches before correction
//     double x = (double) time;
//     return (X_COEF*x + BIAS) * IN2CM;

// }
// /*******************************************************************************
//  * PUBLIC FUNCTIONS FUNCTIONS                                                  *
//  ******************************************************************************/

// /**
//  * @function    PING_Init(void)
//  * @brief       Sets up both the timer extrenal interrupt peripherals along with their
//  *              Also configures PWM_5 as a GPIO output for the trigger pin.
//  * @return      SUCCESS or ERROR
//  */
// char PING_Init(void)
// {
//     // init other libraries
//     BOARD_Init();
//     TIMER_Init();

//     // this block initializes the GPIO output pin (PB8, PWM_5 on shield)
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = GPIO_PIN_8;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//     // this block inits the timer generated interrupt
//     TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//     TIM_MasterConfigTypeDef sMasterConfig = {0};
//     htim3.Instance = TIM3;
//     htim3.Init.Prescaler = 83; // divide by 1 prescaler (84-1) = 1 Mhz tick
//     htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
//     htim3.Init.Period = 0xFFFF; // MUST CHANGE. number of clock cycles between interrupts
//     htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//     htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//     if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
//     {
//         return ERROR;
//     }
//     sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//     if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
//     {
//         return ERROR;
//     }
//     sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//     sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//     if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
//     {
//         return ERROR;
//     }
//     HAL_TIM_Base_Start_IT(&htim3); // start interrupt

//     // this block inits the external pin-generted interrupt on any change of pin PB5 (ENC_B)
//     GPIO_InitStruct.Pin = GPIO_PIN_5;
//     GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//     HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
//     // student added addtions go here
//     CurrentState = Waiting;
//     TimeOfFlight = 0;
//     REdgeRead = 0;
//     FEdgeRead = 0;
//     return SUCCESS;
// }
// /**
//  * @function    PING_GetDistance(void)
//  * @brief       Returns the calculated distance in mm using the sensor model determined
//  *              experimentally.
//  *              No I/O should be done in this function
//  * @return      distance in mm
//  */
// unsigned int PING_GetDistance(void){
//     return (unsigned int) round(TimeFlight2cm(TimeOfFlight));
// }

// /**
//  * @function    PING_GetTimeofFlight(void)
//  * @brief       Returns the raw microsecond duration of the echo from the sensor.
//  *              NO I/O should be done in this function.
//  * @return      time of flight in uSec
//  */
// unsigned int PING_GetTimeofFlight(void){
//     return TimeOfFlight;
// }

// #ifdef PING_TEST
// // tests go here
// int main(void)
// {
//     BOARD_Init();
//     PING_Init();
//     while (TRUE){
//         printf("%u\n",PING_GetTimeofFlight());
//     }
// };
// #endif