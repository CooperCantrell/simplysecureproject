/* 
 * File:   Speaker.c
 * Author: Cooper Cantrell
 * Editor: --
 * Created on 3/14/2025
 * Edited on --
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include <Speaker.h>
#include <config.h>
#include <stdbool.h>
#include <math.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define MIN_FREQ 200
#define MAX_FREQ 1000
#define ADCMAX 4095.0
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static bool INITFLAG = true;
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/
unsigned int ADC2Freq(uint16_t READ){
    return (int) round((((float)READ)/ADCMAX)*(float)((MAX_FREQ-MIN_FREQ)+MIN_FREQ));
}
/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/

// define functions in the .h file

/**
 * @Function SpeakerInit()
 * @param none 
 * @return none
 * @brief INITs the speaker and pot that will read from it
 * @author Cooper Cantrell, 3/14/2025 
*/
void SpeakerInit(void){
    if (INITFLAG)
    {
        PWM_Init();
        ADC_Init();
        PWM_AddPin(SPEAKERPIN);
        SpeakerDuty(0);
    }
    


}

/**
 * @Function SpeakerSetDutyCycle(unsigned int Duty)
 * @param Duty 0-100 Duty Cycle 
 * @return none
 * @brief Sets the Speaker Duty Cycle
 * @author Cooper Cantrell, 3/14/2025 
*/
void SpeakerDuty(unsigned int Duty){
    PWM_SetDutyCycle(SPEAKERPIN,Duty);
}

/**
 * @Function SpeakerUpdate(void)
 * @param none
 * @return none
 * @brief updates the speaker to the duty cycle of the ADC
 * and returns raw ADC
 * @author Cooper Cantrell, 3/14/2025 
*/
uint16_t SpeakerUpdate(void){
    PWM_SetFrequency(ADC2Freq(ADC_Read(POTPIN)));
    return ADC_Read(POTPIN);
}

