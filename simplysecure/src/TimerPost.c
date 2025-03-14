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
// this meth of storeing data is bad can over ride in case [s,s,s,l] 
// s = short wait l = long wait
uint16_t IDs[TIMERPOSTSIZE];
bool full = false; 
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
    TIMER_Init();
    TimerQueue = TODOQueue_init(TIMERPOSTSIZE);
    for (size_t i = 0; i < TIMERPOSTSIZE; i++)
    {
        IDs[i] = -1;
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
    if(!full){
        uint16_t Tracker = 0;
        TODOItem TimerItem;
        for (; Tracker < TIMERPOSTSIZE; Tracker++)
        {
            if (IDs[Tracker] == -1)
            {
                IDs[Tracker] = ID;
                break;
            }
            
        }
        
        TimerItem.Input = (Event){TIMEOUT,IDs+Tracker};
        printf("Added Timer with ID %u, %u, %u and pointer %p, %p\r\n",*(uint16_t*)(IDs+Tracker),IDs[Tracker], ID, &(IDs[Tracker]) ,IDs+Tracker);
        TimerItem.Func = PostItem;
        uint32_t  CurrentTime = TIMERS_GetMilliSeconds();
        //printf("Added Item With ID %p \r\n",&(IDs[Tracker]));
        //HAL_Delay(2);
        full = !EnQueue(TimerQueue,MAX - (Time+CurrentTime),TimerItem);
        return full;
        
    }
    else
    {
        return false;
    }
    
    
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
    u_int16_t ID = *(uint16_t*)GetData(TimerQueue);
    Execute(TimerQueue);
    for (size_t i = 0; i < TIMERPOSTSIZE; i++)
    {
        if (IDs[i] == ID)
        {
            IDs[i] = -1;
            break;
        }
        
    }
    
    
}



}