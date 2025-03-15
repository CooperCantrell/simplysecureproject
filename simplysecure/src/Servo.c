/*******************************************************************************
 * Private INCLUDES                                                           *
 ******************************************************************************/
#include "Servo.h"
#include "config.h"

#define FIFTYHZ 20000
/*******************************************************************************
 * Private Variables                                                           *
 ******************************************************************************/

TIM_HandleTypeDef htim4;

float PWMDC = .5;
uint8_t CurrentState = 0;

/*******************************************************************************
 * Private FUNCTIONS                                                          *
 ******************************************************************************/
void TIM4_IRQHandler(void)
{
    if (__HAL_TIM_GET_IT_SOURCE(&htim4, TIM_IT_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE); // clear interrupt flag

        // state machine of your design
        switch (CurrentState)
        {
        case 0:
        // Timer is 10k Hz
            CurrentState = 1;
            HAL_GPIO_WritePin(GPIOB, SERVO_PIN, GPIO_PIN_SET);
            TIM4->ARR = FIFTYHZ *PWMDC;
            break;

        case 1:
            CurrentState = 0;
            HAL_GPIO_WritePin(GPIOB, SERVO_PIN, GPIO_PIN_RESET);
            TIM4->ARR = FIFTYHZ *(1-PWMDC);

            break;
        default:
            break;
        }
    }
}

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/
/**
 * @Function ServoInit(void)
 * @param none
 * @return ERROR or SUCCESSES
 * @brief inits all of the servo controller module
 * @author Caitlin Bonesio
*/
void ServoInit(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SERVO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 83; // divide by 1 prescaler (84-1) = 1 Mhz tick
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 0xFFFF; // MUST CHANGE. number of clock cycles between interrupts
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
    {
        //return ERROR;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
    {
        //return ERROR;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
        //return ERROR;
    }
    HAL_TIM_Base_Start_IT(&htim4); // start interrupt
}

/**
 * @Function ServoReadDC(void)
 * @param none
 * @return float between 0 and 1 representing the current duty cycle
 * @brief read the current duty cycle
 * @author Caitlin Bonesio
*/
float ServoReadDC(void){
    return PWMDC;
}

/**
 * @Function ServoSetDC(void)
 * @param a float between 0 and 1 representing the next duty cycle
 * @return none
 * @brief sets the current duty cycle
 * @author Caitlin Bonesio
*/
void ServoSetDC(float newDC){
    PWMDC = newDC;
}
