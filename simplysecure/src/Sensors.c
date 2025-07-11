/* 
 * File:   Sensors.h
 * Author: Cooper Cantrell
 * Editor: --
 * Created on 3/8/2025
 * Needed as our sensors share an ISR GPIO5-9
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include <Sensors.h>
#include <TODOQueue.h>
#include <config.h>
#include <stdbool.h>
#include <timers.h>
#include <pwm.h>
#include <Board.h>
#include <Filters.h>
#include <stdio.h>
#include <TimerPost.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define X_COEF 0.006735130021308
#define BIAS 0.239378770600926
#define PINGITCOUNT 10

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
// for captouch sensor ---------------------------------------------------------
volatile uint32_t LastTime;
volatile int Period;
volatile uint32_t CurrentTime;
volatile bool laststate = false;
volatile bool currentstate = false;
// for PING sensor -------------------------------------------------------------
enum States
{
    Waiting,
    Trigger,
};
enum States CurrentState;
uint32_t InterruptCount;
uint32_t TimeOfFlight;
uint32_t REdgeRead; // Riseing edge read
uint32_t FEdgeRead; // Falling
TIM_HandleTypeDef htim3;
double PingDist;
bool PingClose;
Event PingEvent;
// for HAL Sensor --------------------------------------------------------------
bool deboucetimer;
bool HAL_Bounce;
EventLabel HAL_LABEL;
EventLabel HAL_LASTPOST;
// for Servo--------------------------------------------------------------------
static uint16_t DUTY = 50;
enum States ServoState;
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/
// Helper Functions ------------------------------------------------------------
double TimeFlight2in(uint32_t time){
    // this was for inches before correction
    double x = (double) time;
    return (X_COEF*x + BIAS);
}
Event Debounce(Event ThisEvent){
    
    switch (ThisEvent.Label)
    {
    case DOOR_CLOSED:
    case DOOR_OPENED:
        HAL_LABEL = ThisEvent.Label;
        if (!deboucetimer)
        {
            TimerPosting(HAL_DEBOUNCE_WAIT,Debounce,HAL_DEBOUNCE_ID);
            deboucetimer = true;
        }
        else{
            // bouced when timer is on
            HAL_Bounce = true;
        }

        break;
    case TIMEOUT:
        switch (ThisEvent.Data)
        {
        case HAL_DEBOUNCE_ID:
            if (HAL_Bounce)
            {
                HAL_Bounce = false;
                TimerPosting(HAL_DEBOUNCE_WAIT,Debounce,HAL_DEBOUNCE_ID);
                deboucetimer = true;
            }
            else
            {
                deboucetimer = false;
                if((HAL_LASTPOST != HAL_LABEL)){
                    PostUnlockFSM((Event){HAL_LABEL,0},HAL_Priority);
                    HAL_LASTPOST = HAL_LABEL;
                }
            }
            break;
        
        default:
            break;
        }
    default:
        break;
    }
    return NO_EVENT;
}
// ISR -------------------------------------------------------------------------
// Timer ISR -------------------------------------------------------------------
void TIM3_IRQHandler(void)
{
    if (__HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE); // clear interrupt flag

        switch (ServoState)
        {
        case Waiting:
            ServoState = Trigger;
            //printf("w\n");
            HAL_GPIO_WritePin(GPIOB,SERVO_PIN,GPIO_PIN_SET);
            // set_leds(1);
            //TIM3->ARR = 200 * (uint8_t)(DUTY/100);
            //TIM3->ARR = 200 * (uint8_t)((DUTY)/100);
            TIM3->ARR = (uint16_t)(2*DUTY);
            break;
        case Trigger:
            //printf("t %d\n", InterruptCount);
            ServoState = Waiting;
            HAL_GPIO_WritePin(GPIOB,SERVO_PIN,GPIO_PIN_RESET);
            //TIM3->ARR = 200 * (uint8_t)((100-DUTY)/100);
            
            TIM3->ARR = (uint16_t)(2*(100-DUTY));
            // set_leds(0);
            InterruptCount++;
            //PING state machine
            if(InterruptCount == PINGITCOUNT){
                InterruptCount = 0;
                ServoState = Trigger;
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
                TIM3->ARR = 1;
            } else if (InterruptCount == 1) {
                //printf("Ping\n");
                //printf("Ping\n");
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
                // TIM3->ARR = 200 * (uint8_t)((100-DUTY)/100)-1;
                
                TIM3->ARR = (uint16_t)(2*(100-DUTY)-1);
            }
            break;
        default:
            break;
        }
    }
}
// GPIO ISR --------------------------------------------------------------------
void EXTI9_5_IRQHandler(void)
{
    // EXTI line interrupt detected
    // PING ISR
    if (__HAL_GPIO_EXTI_GET_IT(PING_ECHO) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(PING_ECHO); // clear interrupt flag
        // anything that needs to happen when PB5 (ENC_B) changes state
        if(HAL_GPIO_ReadPin(GPIOB,PING_ECHO)){ // trigger on rise edge
            REdgeRead = TIMERS_GetMicroSeconds();
        }
        else{ // fall edge
            FEdgeRead = TIMERS_GetMicroSeconds();
            TimeOfFlight = FEdgeRead-REdgeRead;
            if (!(TimeFlight2in(TimeOfFlight) > PING_MISS_THRESH))
            {
                PingDist = (PingDist/2) + (TimeFlight2in(TimeOfFlight)/2);
            }
            //printf("%f\n",PingDist);
            if ((PingDist<PING_CLOSE_THRESH-PING_CLOSE_TOL) && !PingClose)
            {
                PingClose = true;
                PingEvent.Label = PING_CLOSE;
                PingEvent.Data = 0;
                PostUnlockFSM(PingEvent,PING_Priority);
            }
            else if ((PingDist>PING_CLOSE_THRESH+PING_CLOSE_TOL) && PingClose)
            {
                PingClose = false;
                PingEvent.Label = PING_FAR;
                PingEvent.Data = 0;
                PostUnlockFSM(PingEvent,PING_Priority);
            }
            
            
        }
    }
    // CapTouch ISR
    if (__HAL_GPIO_EXTI_GET_IT(CAPTOUCH_PIN) != RESET)
    {  
         // Clear interrupt flag.
         __HAL_GPIO_EXTI_CLEAR_IT(CAPTOUCH_PIN);
         // Anything that needs to happen on rising edge of PB5
         // (ENC_B).
         // reads the current time in us and compares it to the last riseing edge time 
         // to calculate the period, Filters the result to reduce noise first with a 
         // 255 point moving avg filter first
         CurrentTime = TIMERS_GetMicroSeconds();
         Period = MovingAvgFIT(CurrentTime-LastTime,0);
         LastTime = CurrentTime;
        currentstate = HystFilter256(Period,600,50,0);
        //printf("%i\r\n",Period);
        if (currentstate != laststate)
        {
            if (currentstate)
            {
                PostSimplyFSM(PRESSED,CAPTOUCH_Priority);
            }
            else
            {
                PostSimplyFSM(UNPRESSED,CAPTOUCH_Priority);
            }
            laststate = currentstate;
        }
    }
    if (__HAL_GPIO_EXTI_GET_IT(HAL_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(HAL_PIN);
        TODOItem HALTODO;
        HALTODO.Func = Debounce;
        if (!HAL_GPIO_ReadPin(GPIOB,HAL_PIN))
        {
            HALTODO.Input = (Event){DOOR_CLOSED,0};
        }
        else
        {
            HALTODO.Input = (Event){DOOR_OPENED,0};
        }
        EnQueueMaster(HAL_Priority,HALTODO);  
    }
    
        

     
}
/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/

/**
 * @Function SensorInit(void)
 * @param none
 * @return none
 * @brief inits all of the sensors together so the ISR works better
 * @author Cooper Cantrell, DATE 
 * @edited EDITORNAME, DATE - only if applies
*/
char SensorInit(void){
    /**
     * INIT captouch sensor
     */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = CAPTOUCH_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0,0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    // the rest of the function goes here
    TIMER_Init();
    LastTime = 0;

    BOARD_Init();
    TIMER_Init();

    // this block initializes the GPIO output pin (PB8, PWM_5 on shield)
    GPIO_InitStruct.Pin = PING_TRIG;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // this block inits the timer generated interrupt
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = (uint16_t)(HAL_RCC_GetSysClockFreq()/10000-1); //set frequency to 10000H or 10kHzz
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0xFFFF; //changed later with TIM3->ARR
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
    DUTY = 10;
    ServoState = Trigger;
    TIM3->ARR = 200;
    HAL_TIM_Base_Start_IT(&htim3); // start interrupt

    // this block inits the external pin-generted interrupt on any change of pin PB5 (ENC_B)
    GPIO_InitStruct.Pin = PING_ECHO;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    // student added addtions go here
    CurrentState = Waiting;
    TimeOfFlight = 0;
    REdgeRead = 0;
    FEdgeRead = 0;
    PingDist = 0;
    PingClose = false;
    PingEvent = NO_EVENT;
    // HAL Init
    TimerPostInit();
    GPIO_InitStruct.Pin = HAL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    deboucetimer = false;
    HAL_Bounce = false;
    HAL_LASTPOST = NONE;
    // Servo INIT
    GPIO_InitStruct.Pin = SERVO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    return SUCCESS;
}

/**
 * @Function SERVO_SetDutyCycle(uint16_t duty)
 * @param new duty cycle, must be between 1-99
 * @return none
 * @brief inits all of the sensors together so the ISR works better
 * @author Caitlin Bonesio
 * @edited EDITORNAME, DATE - only if applies
*/
void SERVO_SetDutyCycle(uint16_t duty){
    if(duty < 100 && duty >0){
        DUTY = duty;
    }
}