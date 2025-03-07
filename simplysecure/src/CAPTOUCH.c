// /**
//  * @file    CAPTOUCH.c
//  *
//  * Capacitive touch sensing module
//  *
//  * @author  Cooper Cantrell
//  * @date    1/29/2025
//  */

// /*******************************************************************************
//  * MODULE #INCLUDE                                                             *
//  ******************************************************************************/
// #include <CAPTOUCH.h>
// #include <timers.h>
// #include <Filters.h>
// /*******************************************************************************
//  * PRIVATE #DEFINES                                                            *
//  ******************************************************************************/
// //#define CAP_TEST
// #define CAPTOUCH_PIN GPIO_PIN_5
// /*******************************************************************************
//  * PRIVATE VARIABLES                                                           *
//  ******************************************************************************/
// volatile uint32_t LastTime;
// volatile int Period;
// volatile uint32_t CurrentTime;
// /*******************************************************************************
//  * PRIVATE FUNCTIONS/CLASSES                                                   *
//  ******************************************************************************/
//  // external interrupt ISR for rising edge of pin PB5  
//  void EXTI9_5_IRQHandler(void) {
//      // EXTI line interrupt detected 
//      if(__HAL_GPIO_EXTI_GET_IT(CAPTOUCH_PIN) != RESET) {
//          // Clear interrupt flag.
//          __HAL_GPIO_EXTI_CLEAR_IT(CAPTOUCH_PIN);
//          // Anything that needs to happen on rising edge of CAPTOUCH_PIN
//          // (ENC_B).
//          // reads the current time in us and compares it to the last riseing edge time 
//          // to calculate the period, Filters the result to reduce noise first with a 
//          // 255 point moving avg filter first
//          CurrentTime = TIMERS_GetMicroSeconds();
//          Period = CurrentTime-LastTime;
//          LastTime = CurrentTime;

//      }
//  }
// /*******************************************************************************
//  * PUBLIC FUNCTIONS                                                            *
//  ******************************************************************************/

// /** CAPTOUCH_Init()
//  *
//  * This function initializes the module for use. Initialization is done by
//  * opening and configuring timer 2, opening and configuring the GPIO pin and
//  * setting up the interrupt.
//  */
//  void CAPTOUCH_Init(void) {
//     //Configure GPIO pin PB5 
//     GPIO_InitTypeDef GPIO_InitStruct = {0};
//     GPIO_InitStruct.Pin = GPIO_PIN_5;
//     GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//     // EXTI interrupt init
//     HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
//     // the rest of the function goes here
//     TIMER_Init();
//     LastTime = 0;
//  }

// /** CAPTOUCH_IsTouched(void)
//  *
//  * Returns TRUE if finger is detected. Averaging of previous measurements may
//  * occur within this function, however you are NOT allowed to do any I/O inside
//  * this function.
//  *
//  * @return  (char)    [TRUE, FALSE]
//  */
// char CAPTOUCH_IsTouched(void){
//     // puts the Period into a hysteresis latch whith a threshold of 55us and a tolance of 5us
//     // the 0 indcates that it is useing latch 0 as this function supports up to 255 unquite hyst latches
//     return (char)HystFilter256(Period,55,5,0);
// }

// // testing for QEI
// #ifdef CAP_TEST
// #include<stdio.h>
// int main(void)
// {
//     BOARD_Init();
//     CAPTOUCH_Init();
//     printf("Hello World \n");
//     while(TRUE){
//         if (CAPTOUCH_IsTouched())
//         {
//             printf("PRESSED\n");
//         }
//         else
//         {
//             printf("OFF\n");
//         }
        
//     }
//     return 0;
// }

// #endif