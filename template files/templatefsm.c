/*
 * File: templatefsm.c
 * Authors: Cooper Cantrell, Anirudh Iyer
 * Date 2-08-2025
 *
 * Template for a first order state machine. This works with the todo queue and 
 * the FLARE config file 
 *
 */

 /*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
//#include <templatefsm.h>
//#include TODOQueue.h
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    Init,
    Fstate,
} TemplateFSMState_t;

static const char *StateNames[] = {
	"Init",
	"Fstate",
};


static TemplateFSMState_t CurrentState = Init; // <- change enum name to match ENUM

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/

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
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = Init;
    TODOItem Post;
    Post.Func = RunTemplateFSM;
    Post.Input = INIT_EVENT;
    // post the initial transition event
    return EnQueueMaster(MyPriority, Post);
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
    TemplateFSMState_t nextstate;

    switch (CurrentState)
    {
    case Init:
        /* code */

        Transition = true;
        InputEvent = NO_EVENT;
        nextstate = Fstate;
        break;
    
    case Fstate:
        // code
        break;
    default:
        // code
        break;
    }
    if(Transition){
        RunTemplateFSM(EXIT_EVENT);
        CurrentState = nextstate;
        RunTemplateFSM(ENTRY_EVENT);
    }

    return InputEvent;
 }
