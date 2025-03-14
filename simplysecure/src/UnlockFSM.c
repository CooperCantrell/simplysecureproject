/*
 * File: Unlockfsm.c
 * Authors:Caitlin Bonesio
 * Date 3-12-2025
 *
 * A state machine to interact with the FLARE TODOQueue system to implement the Unlock secure password unlock proccess,
 * Built from a FLARE template 
 *
 */

 /*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include "UnlockFSM.h"
#include "TODOQueue.h"
#include "timers.h"
#include "TimerPost.h"
#include <stdbool.h>
#include <math.h>
#include <leds.h>
#include <stdio.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define GRACEPERIOD 30000
#define TATTLE
/* PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    Init,
    Locked,
    Unlocked,
    Locking,
    ScreamON,
    ScreamOFF,
} UnlockFSMState_t;

static const char *StateNames[] = {
    "Init",
    "Locked",
    "Unlocked",
    "Locking",
    "ScreamON",
    "ScreamOFF",
};

#define UNLOCKFSMTIMERID 50

typedef enum TimerIDs{
    GracePeriod,
    Scream,
}TimerIDs_t;



/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

 static UnlockFSMState_t CurrentState = Init; 

 uint16_t UnlockPriority;


/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/
/**
 * @Function InitSimmplyFSM(uint16_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an INIT event to the appropriate event
 *        queue, which will be handled inside RunUnlockFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Anirudh Iyer 2/8/2025 */
 bool InitUnlockFSM(uint16_t Priority){
    UnlockPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = Init;
    TODOItem Post;
    Post.Func = RunUnlockFSM;
    Post.Input = INIT_EVENT;
    // post the initial transition event
    return EnQueueMaster(UnlockPriority, Post);
 }


 /**
 * @Function PostUnlockFSM(Event inpEvent, unint16_t Priority)
 * @param InpEvent - the event (label and data) to be posted to queue
 * @param Priority - the priority of this event in the todo queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to posting to the master queue
 * @author Cooper Cantrell 2/8/2025 */
 bool PostUnlockFSM(Event InputEvent, uint16_t Priority){
    TODOItem Post;
    Post.Func = RunUnlockFSM;
    Post.Input = InputEvent;
    return EnQueueMaster(Priority,Post);
 }

 /**
  * @Function RunUnlockFSM(Event ThisEvent)
  * @param InputEvent - the event (label and data) to be in the state machine
  * @return Event - return event (label and data), only exist so I can reuse code for
  * the HSM later, should be NO_EVENT 
  * @brief This is a function that you write and rename for your use
  * @author Cooper Cantrell 2/8/2025
  */
 Event RunUnlockFSM(Event InputEvent){
    #ifdef TATTLE
    printf("%s called, Input = %s:  STATE = %s \n", __PRETTY_FUNCTION__, EventNames[InputEvent.Label], StateNames[CurrentState]);
    #endif
    bool Transition = false;
    UnlockFSMState_t nextstate;
    if (InputEvent.Label == PING_CLOSE)
    {
        set_leds(0X10);
    }
    if (InputEvent.Label == PING_FAR)
    {
        set_leds(0);
    }

    switch (CurrentState)
    {
    case Init:
        if(InputEvent.Label == INIT){
            if(HAL_GPIO_ReadPin(GPIOB,HAL_PIN)){
                nextstate = Locked;
                Transition = true;
                InputEvent = NO_EVENT;
            } else {
                nextstate = Unlocked;
                Transition = true;
                InputEvent = NO_EVENT;
            }
        }
        break;
    case Unlocked:
        //ServoControl(false);//some kind of servo control function need to go here
        if(InputEvent.Label == PING_FAR){
            if(HAL_GPIO_ReadPin(GPIOB,HAL_PIN)){
                nextstate = Locked;
                Transition = true;
                InputEvent = NO_EVENT;
            } else {
                nextstate = Locking;
                Transition = true;
                InputEvent = NO_EVENT;
            }
        }
        if(InputEvent.Label == DOOR_CLOSED){
            nextstate = Locked;
            Transition = true;
            InputEvent = NO_EVENT;
        }
        break;
    case Locking:
        if(InputEvent.Label == ENTRY){
            TimerPosting(30000, RunUnlockFSM, UNLOCKFSMTIMERID+GracePeriod);
        }
        if(InputEvent.Label == DOOR_CLOSED){
            nextstate = Locked;
            Transition = true;
            InputEvent = NO_EVENT;
        }
        if(InputEvent.Label == TIMEOUT && *(uint16_t*)InputEvent.Data == UNLOCKFSMTIMERID+GracePeriod){
            
            nextstate = ScreamON;
            Transition = true;
            InputEvent = NO_EVENT;
        }
        break;
    case ScreamON:
        if(InputEvent.Label == ENTRY){
            TimerPosting(500, RunUnlockFSM, UNLOCKFSMTIMERID+Scream);
        }
        if(InputEvent.Label == TIMEOUT && *(uint16_t*)InputEvent.Data == UNLOCKFSMTIMERID+Scream){
            nextstate = ScreamOFF;
            Transition = true;
            InputEvent = NO_EVENT;
        }
        break;
    case ScreamOFF:
        if(InputEvent.Label == ENTRY){
            TimerPosting(500, RunUnlockFSM, Scream);
        }
        if(InputEvent.Label == TIMEOUT && *(uint16_t*)InputEvent.Data == UNLOCKFSMTIMERID+Scream){
            nextstate = ScreamON;
            Transition = true;
            InputEvent = NO_EVENT;
        }
        break;
    default:
        break;
    }
    if(Transition){
        RunUnlockFSM(EXIT_EVENT);
        #ifdef TATTLE
        printf("Transition: %s -> %s \n", StateNames[CurrentState], StateNames[nextstate]);
        #endif
        CurrentState = nextstate;
        RunUnlockFSM(ENTRY_EVENT);
    }

    return InputEvent;
 }

