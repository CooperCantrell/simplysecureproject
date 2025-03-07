/*
 * File: Simplyfsm.c
 * Authors: Cooper Cantrell, Anirudh Iyer, Caitlin Bonesio
 * Date 3-06-2025
 *
 * A state machine to interact with the FLARE TODOQueue system to implement the simply secure password unlock proccess,
 * Built from a FLARE template 
 *
 */

 /*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include "SimplyFSM.h"
#include "TODOQueue.h"
#include "timers.h"
#include <stdbool.h>
#include <math.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define STARTCYCLES 3//number of taps and pauses need to calibrate the passcode tempo
#define DEFAULTTEMPO 1000//default pause and tap is one second long
#define MAXCODELENGTH 50//maximun number of taps and waits combined in a code
#define LENIENCY 100// how many milliseconds the user is allowed to be off from the correct time
/* PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    Init,
    MeasureON,
    MeasureOFF,
    ON,
    OFF,
    Unlock,
    Reset,
} SimplyFSMState_t;

static const char *StateNames[] = {
	"Init",
    "MeasureON",
    "MeasureOFF",
    "ON",
    "OFF",
    "Unlock",
    "Reset",
};


static SimplyFSMState_t CurrentState = Init; 

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

 uint16_t SimplyPriority;

 uint32_t onTime[STARTCYCLES];
 uint32_t offTime[STARTCYCLES];
 static float OffTempo = 1;
 static float OnTempo = 1;
//null terminated list of the delays between the ons and the offs of the passcode, the second row is the associated pitch value
 uint32_t PassCode[MAXCODELENGTH][2];
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/
uint32_t recordTime(bool initFlag){
    static uint32_t startTime = 0;
    if(initFlag){
        startTime = TIMERS_GetMicroSeconds();
        return 0;
    } else {
        uint32_t curTime = TIMERS_GetMilliSeconds();
        uint32_t deltaTime = curTime - startTime;
        startTime = curTime;
        return deltaTime;
    }
}

/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/
/**
 * @Function InitTemplateFSM(uint16_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Anirudh Iyer 2/8/2025 */
 bool InitTemplateFSM(uint16_t Priority){
    SimplyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = Init;
    TODOItem Post;
    Post.Func = RunTemplateFSM;
    Post.Input = INIT_EVENT;
    // post the initial transition event
    return EnQueueMaster(SimplyPriority, Post);
 }


 /**
 * @Function PostTemplateFSM(Event inpEvent, unint16_t Priority)
 * @param InpEvent - the event (label and data) to be posted to queue
 * @param Priority - the priority of this event in the todo queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to posting to the master queue
 * @author Cooper Cantrell 2/8/2025 */
 bool PostTemplateFSM(Event InputEvent, uint16_t Priority){
    TODOItem Post;
    Post.Func = RunTemplateFSM;
    Post.Input = InputEvent;
    return EnQueueMaster(Priority,Post);
 }

 /**
  * @Function RunTemplateFSM(Event ThisEvent)
  * @param InputEvent - the event (label and data) to be in the state machine
  * @return Event - return event (label and data), only exist so I can reuse code for
  * the HSM later, should be NO_EVENT 
  * @brief This is a function that you write and rename for your use
  * @author Cooper Cantrell 2/8/2025
  */
 Event RunTemplateFSM(Event InputEvent){
    bool Transition = false;
    SimplyFSMState_t nextstate;
    //count is used to run the start cycle and index through the passcode
    static uint8_t count = 0;
    switch (CurrentState)
    {
    case Init:
        /* code */
        if(InputEvent.Label == CAP_ON){
            recordTime(true);
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = MeasureON;

        }
        if(InputEvent.Label == INIT){
            TIMER_Init();
            count = 0;
        }
        break;
    
    case MeasureON:
        if(InputEvent.Label == CAP_OFF){
            onTime[count] = recordTime(false);
            count++;
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = MeasureOFF;
        }
        break;
    case MeasureOFF:
    
        if(InputEvent.Label == CAP_ON){
            offTime[count] = recordTime(false);
            if(count >= STARTCYCLES){
                count = 0;
                int onSum = 0;
                int offSum = 0;
                for(int i = 0; i < STARTCYCLES; i++){
                    onSum += onTime[i];
                    offSum += offTime[i];
                }
                OnTempo = DEFAULTTEMPO*(STARTCYCLES/(onSum));
                OffTempo = DEFAULTTEMPO*(STARTCYCLES/(offSum));

                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = ON;
            } 

            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = MeasureOFF;
        }
        break;
    case ON:
        if(InputEvent.Label == CAP_OFF){
            uint32_t time = OnTempo * recordTime(false);
            if(count >= MAXCODELENGTH){//shouldn't happen, but prevents crashing
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Reset;
            }
            
            if(PassCode[count][0] == 0){//succeessfully reached the end of the password
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Unlock;
            }
            if(abs((int64_t)time - (int64_t)PassCode[count][0]) <  OnTempo*LENIENCY){//successful tap
                //TODO: add frequency check
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = OFF;
            } else {//failed tap
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Reset;
            }

        }
        break;
    case OFF:
        if(InputEvent.Label == CAP_ON){
            uint32_t time = OnTempo * recordTime(false);
            if(count >= MAXCODELENGTH){//shouldn't happen, but prevents crashing
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Reset;
            }
            
            if(PassCode[count][0] == 0){//succeessfully reached the end of the password
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Unlock;
            }
            if(abs((int64_t)time - (int64_t)PassCode[count][0]) <  OffTempo*LENIENCY){//successful tap
                //TODO add frequency checks
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = OFF;
            } else {//failed tap
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Reset;
            }

        }
        break;
    case Unlock:
        if(InputEvent.Label == ENTRY){
            //Heres where I would post to the UnlockSM IF I HAD ONE
            
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = Reset;
        }
        break;
    case Reset:
        if(InputEvent.Label == ENTRY){
            OnTempo = 1;
            OffTempo = 1;
            count = 0;
            for( int i = 0; i < STARTCYCLES; i++){
                offTime[i] = 0;
                onTime[i] = 0;
            }
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = Init;
        }
        break;
    default:
        break;
    }
    if(Transition){
        RunTemplateFSM(EXIT_EVENT);
        CurrentState = nextstate;
        RunTemplateFSM(ENTRY_EVENT);
    }

    return InputEvent;
 }

