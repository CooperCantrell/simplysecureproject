/* 
 * File:   TimerPost.h
 * Author: Cooper Cantrell
 * Editor: --
 * Created on 3/6/2025
 * Edited on --
 */
#ifndef TimerPost_H  // <- This should be changed to your own guard on both
#define TimerPost_H //    of these lines

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include <inttypes.h>
#include <Events.h>
#include <stdbool.h>
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
 * @Function TimerPostInit(void)
 * @param none 
 * @return none
 * @brief inits all that is needed for the posting to work
 * @author Cooper Cantrell, 3/6/2025 
*/
void TimerPostInit(void);


 /**
 * @Function TimerPosting(uint32_t Time, Event(*PostItem)(Event), uint16_t ID)
 * @param Time - the amount of time in ms that will wait before posting to the state machine 
 * @param PostItem - the function that will be called when the timer runs out this shoud be 
 * a run state machine functon
 * @param ID - somethig to label the timer will be Event.data if called
 * @return bool true if timer event added false otherwise
 * @brief This function adds a timer to the running list
 * @author Cooper Cantrell, 3/6/2025 
*/
bool TimerPosting(uint32_t Time, Event(*PostItem)(Event), uint16_t ID);

/**
 * @Function runtimer(void)
 * @param none 
 * @return none
 * @brief runs the timer
 * @author Cooper Cantrell, 3/6/2025 
*/
void runtimer(void);
#endif 