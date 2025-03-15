/* 
 * File:   Speaker.h
 * Author: Cooper Cantrell
 * Editor: --
 * Created on 3/14/2025
 * Edited on --
 */
#ifndef SPEAKER_H  
#define SPEAKER_H 

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include <ADC.h>
#include <pwm.h>

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
/**
 * @Function SpeakerInit()
 * @param none 
 * @return none
 * @brief INITs the speaker and pot that will read from it
 * @author Cooper Cantrell, 3/14/2025 
*/
void SpeakerInit(void);

/**
 * @Function SpeakerSetDutyCycle(unsigned int Duty)
 * @param Duty 0-100 Duty Cycle 
 * @return none
 * @brief Sets the Speaker Duty Cycle
 * @author Cooper Cantrell, 3/14/2025 
*/
void SpeakerDuty(unsigned int Duty);

/**
 * @Function SpeakerUpdate(void)
 * @param none
 * @return none
 * @brief updates the speaker to the duty cycle of the ADC
 * and returns raw ADC
 * @author Cooper Cantrell, 3/14/2025 
*/
uint16_t SpeakerUpdate(void);

#endif 