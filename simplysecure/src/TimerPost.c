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
static uint32_t endtimes[TIMERPOSTSIZE];
static Event(*endItems[TIMERPOSTSIZE])(Event);
static Event endParams[TIMERPOSTSIZE];
static uint16_t endID[TIMERPOSTSIZE];
uint16_t head,tail;
bool full;
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
    head = 0;
    tail = 0;
    full = false;
    for (uint16_t i = 0; i < TIMERPOSTSIZE; i++)
    {
        endParams[i].Label = TIMEOUT;
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
bool TimerPosting(uint32_t Time, Event(*PostItem)(Event), uint16_t ID){
    if (!full)
    {
        uint32_t currenttime = TIMERS_GetMilliSeconds();
        endtimes[tail] = currenttime + Time;
        endItems[tail] = PostItem;
        endID[tail] = ID;
        endParams[tail].Data = &endID[tail];
        tail++;
        tail = tail%TIMERPOSTSIZE;
        full = (tail == head);
        
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
    uint16_t count = head;
    uint32_t currenttime = TIMERS_GetMilliSeconds();
    if (!full)
    {
    while (count != tail)
    {
        if(endtimes[count] <= currenttime){
            
            head++;
            head = head%TIMERPOSTSIZE;
            full = false;
            endItems[count](endParams[count]);
        }
        
        
        count++;
        count = count%TIMERPOSTSIZE;
    }
    }
    else
    {
        for (uint16_t i = 0; i < TIMERPOSTSIZE; i++)
        {
        if(endtimes[i] <= currenttime){
            
            head++;
            head = head%TIMERPOSTSIZE;
            full = false;
            endItems[i](endParams[i]);
        }
        }
        
    }


}