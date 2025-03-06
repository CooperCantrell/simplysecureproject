#include <CAPTOUCH.h>
#include <timers.h>
#include <Settings.h>

#define CAPTHRESH 333

static volatile uint32_t lastcap;
static volatile uint32_t captime;
static volatile uint8_t capstate;

void CAPTOUCH_Init(void) {
    //Configure GPIO pin PB5 
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    TIMER_Init();

    // the rest of the function goes here
}
#if Part == 3
// external interrupt ISR for rising edge of pin PB5  
void EXTI9_5_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
        // Clear interrupt flag.
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
        if (capstate){//alternate between the start and the end state of a cycle 
            lastcap = TIMERS_GetMicroSeconds();
            capstate = !capstate;
        } else {
            captime = TIMERS_GetMicroSeconds() - lastcap;
            capstate = !capstate;
        }
        // Anything that needs to happen on rising edge of PB5
        // (ENC_B).
    }
}
#endif
/** CAPTOUCH_IsTouched(void)
 *
 * Returns TRUE if finger is detected. Averaging of previous measurements may
 * occur within this function, however you are NOT allowed to do any I/O inside
 * this function.
 *
 * @return  (char)    [TRUE, FALSE]
 */
char CAPTOUCH_IsTouched(void){
    return captime >= CAPTHRESH;
}