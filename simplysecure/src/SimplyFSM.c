/*
 * File: Simplyfsm.c
 * Authors:Caitlin Bonesio
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
#include "TimerPost.h"
#include "UnlockFSM.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <leds.h>
#include <stdio.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define STARTCYCLES 3//number of taps and pauses need to calibrate the passcode tempo
#define DEFAULTTEMPO 1000000//default pause and tap is one second long
#define MAXCODELENGTH 50//maximun number of taps and waits combined in a code
#define LENIENCY 1000000// how many microseconds the user is allowed to be off from the correct time
#define SIMPLYTIMERID 1//The ID used for the Timer posting
#define UNLOCKEVENT_PRIORITY 3
//#define TATTLE
/* PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    Init,
    MeasureON,
    MeasureOFF,
    Wait,
    ON,
    OFF,
    Unlock,
    Reset,
} SimplyFSMState_t;

static const char *StateNames[] = {
	"Init",
    "MeasureON",
    "MeasureOFF",
    "Wait",
    "ON",
    "OFF",
    "Unlock",
    "Reset",
};



/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

 static SimplyFSMState_t CurrentState = Init; 

 static bool passwordSetMode = false;

 uint16_t SimplyPriority;
uint16_t Heartbeat=0;
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
        uint32_t curTime = TIMERS_GetMicroSeconds();
        uint32_t deltaTime = curTime - startTime;
        startTime = curTime;
        return deltaTime;
    }
}

/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/
/**
 * @Function InitSimmplyFSM(uint16_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an INIT event to the appropriate event
 *        queue, which will be handled inside RunSimplyFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Anirudh Iyer 2/8/2025 */
 bool InitSimplyFSM(uint16_t Priority){
    SimplyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = Init;
    TODOItem Post;
    Post.Func = RunSimplyFSM;
    Post.Input = INIT_EVENT;
    // post the initial transition event
    return EnQueueMaster(SimplyPriority, Post);
 }


 /**
 * @Function PostSimplyFSM(Event inpEvent, unint16_t Priority)
 * @param InpEvent - the event (label and data) to be posted to queue
 * @param Priority - the priority of this event in the todo queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to posting to the master queue
 * @author Cooper Cantrell 2/8/2025 */
 bool PostSimplyFSM(Event InputEvent, uint16_t Priority){
    TODOItem Post;
    Post.Func = RunSimplyFSM;
    Post.Input = InputEvent;
    return EnQueueMaster(Priority,Post);
 }

 /**
  * @Function RunSimplyFSM(Event ThisEvent)
  * @param InputEvent - the event (label and data) to be in the state machine
  * @return Event - return event (label and data), only exist so I can reuse code for
  * the HSM later, should be NO_EVENT 
  * @brief This is a function that you write and rename for your use
  * @author Cooper Cantrell 2/8/2025
  */
 Event RunSimplyFSM(Event InputEvent){

    // if (InputEvent.Label == CAP_ON)
    // {
    //     set_leds(1);
    // }
    // if (InputEvent.Label == CAP_OFF)
    // {
    //     set_leds(0);
    // }
    bool Transition = false;
    SimplyFSMState_t nextstate;
    //count is used to run the start cycle and index through the passcode
    #ifdef TATTLE
    printf("%s called, Input = %s:  STATE = %s DATA = %u\n", __PRETTY_FUNCTION__, EventNames[InputEvent.Label], StateNames[CurrentState],InputEvent.Data);
    #endif
    
    if (((InputEvent.Data)==HEARTID)){
        TimerPosting(1000,RunSimplyFSM,HEARTID);
        
        // printf("Ba-Bum x%u\r\n",Heartbeat++);
        // printf("ID: %u\r\n", InputEvent.Data);
        InputEvent = NO_EVENT;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3));
        //HAL_GPIO_WritePin(GPIOB, SERVO_PIN, !HAL_GPIO_ReadPin(GPIOB, SERVO_PIN));
    }
    static uint8_t count = 0;
    switch (CurrentState)
    {
    case Init:
        if(InputEvent.Label == BUTTON){
            passwordSetMode = true;
        }
        if(InputEvent.Label == TIMEOUT && InputEvent.Data == SIMPLYTIMERID){
            set_leds(0x00);
        }
        if(InputEvent.Label == CAP_ON){
            recordTime(true);//Reset and start the record time function
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = MeasureON;
            set_leds(0x01);

        }
        if(InputEvent.Label == INIT){
            TIMER_Init();//should be done earlier, however, initializing the timers twice wont hurt as it has protections
            count = 0;
            passwordSetMode = true;//When device first boots up, set the password
            TimerPosting(1000,RunSimplyFSM,HEARTID);
        }
        break;
    
    case MeasureON://Provides a test cycle to measure the users tempo, so the same rythm can be played with differnt tempos and still work
        if(InputEvent.Label == CAP_OFF){
            onTime[count] = recordTime(false);//record the duration the user held the capacitor on
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = MeasureOFF;
        }
        break;
    case MeasureOFF://provides the second part of the test cycle
    
        if(InputEvent.Label == CAP_ON){
            offTime[count] = recordTime(false);// records the time the user held off the capcitor
            count++;//move to next test cycle
            if(count >= STARTCYCLES){//provides the logic for leaving the test cycle after STARTCYCLE cycles have passed
                count = 0;//reset count for use for iterating through the password array

                //calculate the averge on and off time for the tempo
                uint64_t onSum = 0;
                uint64_t offSum = 0;
                for(int i = 0; i < STARTCYCLES; i++){
                    //printf("onTime: %ld, offTime: %ld\r\n", onTime[i], offTime[i]);
                    onSum += onTime[i];
                    offSum += offTime[i];
                }
                //provides a scaler that converts from the users current tempo to the standard tempo of 1 second per period
                OnTempo = DEFAULTTEMPO*(STARTCYCLES/((float)onSum));
                OffTempo = DEFAULTTEMPO*(STARTCYCLES/((float)offSum));
                //printf("Ontempo: %f, Offtempo: %f\r\n", OnTempo, OffTempo);

                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Wait;
            } else{

                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = MeasureON;
            }
        }
        break;
    case Wait:
        if(InputEvent.Label == ENTRY){
            
            set_leds(0x03);
        }
        if(InputEvent.Label == CAP_ON){
            recordTime(true);
            set_leds(0x07);
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = ON;
        }
        break;
    case ON://measures and compares the users on period at to the passcode at index count
        if(InputEvent.Label == CAP_OFF){
            set_leds(0x03);
            uint32_t time = OnTempo * recordTime(false);//converts user time to standard time
            if(count >= MAXCODELENGTH){//shouldn't happen, but prevents crashing
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Reset;
            }
            if(passwordSetMode){
                PassCode[count][0] = time;
                //PassCode[count][1] = //Heres where I would read the frequency IF I HAD IT
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = OFF;
            } else {
                if(PassCode[count][0] == 0){//succeessfully reached the null terminator the end of the password
                    Transition = true;
                    InputEvent = NO_EVENT;
                    nextstate = Unlock;
                }
                if(abs((int64_t)time - (int64_t)PassCode[count][0]) <  (int64_t)(OnTempo*LENIENCY)){//successful tap
                    //TODO: add frequency check
                    Transition = true;
                    InputEvent = NO_EVENT;
                    nextstate = OFF;
                } else {//failed tap
                    printf("expected time: %ld, recieved: %ld\r\n", PassCode[count][0], time);
                    Transition = true;
                    InputEvent = NO_EVENT;
                    nextstate = Reset;
                    set_leds(0);
                }
            }
            count++;

        }
        break;
    case OFF:
        
        if(InputEvent.Label == ENTRY){//creates a 3 second timeout timer
            if(passwordSetMode){
                TimerPosting(3000, RunSimplyFSM, count);
            }
        }
        if(InputEvent.Label == TIMEOUT && InputEvent.Data == count){//implements the stop of the new passcode
            PassCode[count][0] = 0;
            PassCode[count][1] = 0;
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = Unlock;
            // for(int i = 0; PassCode[i][0] != 0 && i < MAXCODELENGTH; i++){
            //     printf("Passcode[%d]: %ld\r\n", i, PassCode[i][0]);
            // }
            // printf("count: %d\r\n", count);
            // printf("Passcode[%d]: %ld\r\n", count, PassCode[count][0]);
        }
        if(InputEvent.Label == CAP_ON){
            set_leds(0x07);
            uint32_t time = OnTempo * recordTime(false);
            if(count >= MAXCODELENGTH){//shouldn't happen in check mode, but prevents crashing in set password mode
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = Reset;
            }
            if(passwordSetMode){//swab behavoir from checking to recording based on if the password is being set
                PassCode[count][0] = time;
                //PassCode[count][1] = //Heres where I would put the frequency IF I HAD IT
                Transition = true;
                InputEvent = NO_EVENT;
                nextstate = ON;
            } else {

                if(PassCode[count][0] == 0){//succeessfully reached the end of the password
                    Transition = true;
                    InputEvent = NO_EVENT;
                    nextstate = Unlock;
                } else if(abs((int64_t)time - (int64_t)PassCode[count][0]) <  OffTempo*LENIENCY){//successful tap
                    //TODO add frequency checks
                    Transition = true;
                    InputEvent = NO_EVENT;
                    nextstate = ON;
                } else {//failed tap
                    set_leds(0);
                    Transition = true;
                    InputEvent = NO_EVENT;
                    nextstate = Reset;
                }
            }
            count++;

        }
        break;
    case Unlock:
        if(InputEvent.Label == ENTRY){
            TimerPosting(500, RunSimplyFSM, SIMPLYTIMERID);

            set_leds(0xFF);
            static Event UnlockEvent;
            UnlockEvent.Label = UNLOCK;
            UnlockEvent.Data = 42;
            PostUnlockFSM(UnlockEvent, UNLOCKEVENT_PRIORITY);
            
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
            passwordSetMode = false;
            Transition = true;
            InputEvent = NO_EVENT;
            nextstate = Init;
        }
        break;
    default:
        break;
    }
    if(Transition){
        RunSimplyFSM(EXIT_EVENT);
        #ifdef TATTLE
        printf("Transition: %s -> %s \n", StateNames[CurrentState], StateNames[nextstate]);
        #endif
        CurrentState = nextstate;
        RunSimplyFSM(ENTRY_EVENT);
    }

    return InputEvent;
 }

