/*
 * File:   servo.h
 * Author: Caitlin 
 * Editor Cooper Cantrell
 * Created on 3/14/2025
 * Edited on 3/14/2025
 * EDIT 
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include <Servos.h>
#include <config.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
TIM_HandleTypeDef htim5;
static uint8_t DUTY;
enum States
{
    OFF,
    ON,
};
enum States CurrentState;
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/
#include <stdio.h>
void TIM5_IRQHandler(void)
{
    if (__HAL_TIM_GET_IT_SOURCE(&htim5, TIM_IT_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE); // clear interrupt flag
        printf("SERV\n");
        switch (CurrentState)
        {
        case OFF:
            CurrentState = ON;
            HAL_GPIO_WritePin(GPIOA,SERVO_PIN,GPIO_PIN_SET);
            TIM5->ARR = DUTY;
            break;
        case ON:
            CurrentState = OFF;
            HAL_GPIO_WritePin(GPIOA,SERVO_PIN,GPIO_PIN_RESET);
            TIM5->ARR = 100-DUTY;
            break;
        default:
            break;
        }
    }
}
// void TIM3_IRQHandler(void)
// {
//     if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET)
//     {
//         __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag

//         // state machine of your design
//         switch (CurrentState)
//         {
//         case Waiting:
//         // Timer is 10k Hz
//             CurrentState = Trigger;
//             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
//             TIM3->ARR = 1;
//             break;

//         case Trigger:
//             CurrentState = Waiting;
//             HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
//             TIM3->ARR = 1000;

//             break;
//         default:
//             break;
//         }
//     }
// }
/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/

// define functions in the .h file

/**
 * @Function ServoInit(void)
 * @param none
 * @return SUCCESS or ERROR
 * @brief INITS the servo module
 * @author CooperCantrell, 3/14/2025 
*/
char ServoInit(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SERVO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim5.Instance = TIM5;
    htim5.Init.Prescaler = ((83));//*1000000)/(SERVO_FREQ*100); // 50hz/100
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 100; // MUST CHANGE. number of clock cycles between interrupts
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
    {
        return ERROR;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
    {
        return ERROR;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
    {
        return ERROR;
    }
    HAL_TIM_Base_Start_IT(&htim5); // start interrupt

    DUTY = 50;
    CurrentState = OFF;
    return SUCCESS;
}
/**
 * @Function ServoDuty(void)
 * @param Duty - 0 to 100 repersenting % Duty cycle
 * @return none
 * @brief sets the Duty cycle of the servo
 * @author CooperCantrell, 3/14/2025 
*/
void ServoDuty(uint8_t Duty){
    DUTY = Duty;
}


