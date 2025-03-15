 /*
 * File: filename.c
 * Author: --
 * Modified: --
 *
 * FILE descption other notes
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include <TimerPost.h>
#include <config.h>
#include <Events.h>
#include <timers.h>
#include <TODOQueue.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
// #define FILENAME_TEST
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
TODOList TimerQueue;
uint32_t MAX = -1;
static bool initflag = true;
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/
/**
 * @Function TimerPostInit(void)
 * @param none 
 * @return none
 * @brief inits all that is needed for the posting to work
 * @author Cooper Cantrell, 3/6/2025 
*/
void TimerPostInit(void){
    if (initflag)
    {
    initflag = false;
    TIMER_Init();
    TimerQueue = TODOQueue_init(TIMERPOSTSIZE);
    }
}

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
#include <stdio.h>
bool TimerPosting(uint32_t Time, Event(*PostItem)(Event), uint16_t ID){

    TODOItem TimerItem;
    TimerItem.Input = (Event){TIMEOUT,ID};
    TimerItem.Func = PostItem;
    uint32_t  CurrentTime = TIMERS_GetMilliSeconds();
    //printf("Added Item With ID %p \r\n",&(IDs[Tracker]));
    return EnQueue(TimerQueue,MAX - (Time+CurrentTime),TimerItem);

    
    
}

/**
 * @Function runtimer(void)
 * @param none 
 * @return none
 * @brief runs the timer
 * @author Cooper Cantrell, 3/6/2025 
*/

void runtimer(void){
uint32_t  CurrentTime = TIMERS_GetMilliSeconds();
if (CurrentTime>=(MAX - ReadPriorty(TimerQueue)))
{
    Execute(TimerQueue);
}



}