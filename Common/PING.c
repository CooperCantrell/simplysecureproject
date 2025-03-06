#include <PING.h>
#include <Settings.h>

static volatile uint32_t PingStart = 0;
static volatile uint32_t LastPing = 0;

char PING_Init(void) {
    // init other libraries
    BOARD_Init();
    TIMER_Init();
    // this block initializes the GPIO output pin (PB8, PWM_5 on shield)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // this block inits the timer generated interrupt
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 83; // divide by 1 prescaler (84-1) = 1 Mhz tick
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0xFFFF; // MUST CHANGE. number of clock cycles between interrupts
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        return ERROR;
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        return ERROR;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        return ERROR;
    }
    HAL_TIM_Base_Start_IT(&htim3); // start interrupt
    // this block inits the external pin-generted interrupt on any change of pin PB5 (ENC_B)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    TIM3->ARR = 60000;//interrupt in 60 ms
    return SUCCESS;
}
#if Part == 2
// external interrupt ISR for pin PB5 
//Measures on time of Ping sensor signal
void EXTI9_5_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)){
            //Rising edge case
            PingStart = TIMERS_GetMicroSeconds();
        } else {
            //falling edge
            LastPing = TIMERS_GetMicroSeconds() - PingStart;
        }
    }
}
#endif
static uint8_t state = 0;
// TIM3 ISR
//Implements a PWM signal with a 60ms period and a 10 microsecond duty cycle
void TIM3_IRQHandler(void) {
    if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET) {
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag
        // state machine of your design
        if(state){
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
            //wait 10 microseconds
            TIM3->ARR = 10;
            state = 0;
        } else {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
            //wait 60 ms
            TIM3->ARR = 60000;
            state = 1;
        }
    }
 }

 /**
 * @function    PING_GetDistance(void)
 * @brief       Returns the calculated distance in mm using the sensor model determined
 *              experimentally. 
 *              No I/O should be done in this function
 * @return      distance in mm
 */
unsigned int PING_GetDistance(void){
    //provides distance accurate to a cm
    return LastPing *340/2/1000;
}

/**
 * @function    PING_GetTimeofFlight(void)
 * @brief       Returns the raw microsecond duration of the echo from the sensor.
 *              NO I/O should be done in this function.
 * @return      time of flight in uSec
 */
unsigned int PING_GetTimeofFlight(void){
    return LastPing;
}