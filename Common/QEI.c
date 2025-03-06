#include <QEI.h>
#include <Settings.h>

typedef struct QEISM
{
    uint8_t state;
    int16_t angle; 
}QEISM_t;

static volatile QEISM_t QEISM = {0, 0};

void QEI_Init(void) {
    //Configure GPIO pins : PB4 PB5 
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // the rest of the function goes here
}

/**
 * @Function QEI_IRQ(char edge) 
 * @param  edge, a char that should hold either 'A', or 'B' indicating the edge transition the IRQ has measured
 * @return none
 * @brief  implements the state machine to read from a QEI encoder. The encoer
*/
void QEI_IRQ(char edge) {
    //state machine of your design
    switch (QEISM.state){
        case 0b00://both signals are off
            if(edge == 'A' && HAL_GPIO_ReadPin(GPIOB, ENC_A)){//A rises
                QEISM.state = 0b01;
                QEISM.angle == 96 ? QEISM.angle = 0:QEISM.angle++;//implements a quick mod function where the device has 96 states per rotation
            } else if(HAL_GPIO_ReadPin(GPIOB, ENC_B)){//B rises
                QEISM.state = 0b10;
                QEISM.angle == -96 ? QEISM.angle = 0:QEISM.angle--;
            }
            break;
        case 0b01://A is on B is off
            if(edge == 'A' && !HAL_GPIO_ReadPin(GPIOB, ENC_A)){//A falls
                QEISM.state = 0b00;
                QEISM.angle == -96 ? QEISM.angle = 0:QEISM.angle--;
            } else if(HAL_GPIO_ReadPin(GPIOB, ENC_B)){//B rises
                QEISM.state = 0b11;
                QEISM.angle == 96 ? QEISM.angle = 0:QEISM.angle++;
            }
            break;
        case 0b10://A is off B is on
            if(edge == 'A' && HAL_GPIO_ReadPin(GPIOB, ENC_A)){//A rises
                QEISM.state = 0b11;
                QEISM.angle == -96 ? QEISM.angle = 0:QEISM.angle--;
            } else if(!HAL_GPIO_ReadPin(GPIOB, ENC_B)){//B falls
                QEISM.state = 0b00;
                QEISM.angle == 96 ? QEISM.angle = 0:QEISM.angle++;
            }
            break;
        case 0b11://Both are high
            if(edge == 'A' && !HAL_GPIO_ReadPin(GPIOB, ENC_A)){//A falls
                QEISM.state = 0b10;
                QEISM.angle == 96 ? QEISM.angle = 0:QEISM.angle++;
            } else if(!HAL_GPIO_ReadPin(GPIOB, ENC_B)){//B falls
                QEISM.state = 0b01;
                QEISM.angle == -96 ? QEISM.angle = 0:QEISM.angle--;
            }
            break;
    }
}
#if Part == 1
// external interrupt ISR for pin PB5 
void EXTI9_5_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag
        QEI_IRQ('B');
    }
}
#endif
// external interrupt ISR for pin PB4
void EXTI4_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4); // clear interrupt flag
        QEI_IRQ('A');
    }
}

 

 /**
 * @function QEI_GetPosition(void) 
 * @param none
 * @brief This function returns the current position of the Quadrature Encoder in degrees.      
*/
int QEI_GetPosition(void){
    return QEISM.angle*360/96;
}

/**
 * @Function QEI_ResetPosition(void) 
 * @param  none
 * @return none
 * @brief  Resets the encoder such that it starts counting from 0.
*/
void QEI_ResetPosition(){
    QEISM.angle = 0;
    QEISM.state = 0;
}