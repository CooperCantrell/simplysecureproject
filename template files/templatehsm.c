/*
 * File: templatehsm.c
 * Authors: Anirudh Iyer
 * Date 2-08-2025
 *
 * Template for a hierarchical state machine. This works with the TODOqueue and 
 * the FLARE config file 
 *
 */

 /*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include <templatehsm.h>
#include <stdio.h>
#include <stdlib.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
typedef enum {
    Init,
    Hstate,
} TemplateHSMState_t;

static const char *StateNames[] = {
	"Init",
	"Hstate",
};


static TemplateHSMState_t CurrentState = Init; // <- change enum name to match ENUM
static uint16_t MyPriority;

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
 * @Function InitTemplateHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Anirudh Iyer 2/8/2025 */
bool InitTemplateHSM(uint16_t Priority)
{
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = Init;
    TODOItem Post;
    Post.Func = RunTemplateHSM;
    Post.Input = INIT_EVENT;
    // post the initial transition event
    return EnQueueMaster(MyPriority, Post);
}

/**
 * @Function PostTemplateHSM(Event inpEvent, unint16_t Priority)
 * @param InputEvent - the event (label and data) to be posted to queue
 * @param Priority - the priority of this event in the todo queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to posting to the master queue
 * @author Anirudh Iyer 2/8/2025 */
 bool PostTemplateHSM(Event InputEvent, uint16_t Priority){
    TODOItem Post;
    Post.Func = RunTemplateHSM;
    Post.Input = InputEvent;
    return EnQueueMaster(Priority, Post);
 }

 /**
  * @Function RunTemplateHSM(Event ThisEvent)
  * @param InputEvent - the event (label and data) to be in the state machine
  * @return Event - return event (label and data), should be NO_EVENT 
  * @brief This is a function that you write and rename for your use
  * @author Anirudh Iyer 2/8/2025
  */
 Event RunTemplateHSM(Event InputEvent){
    bool Transition = false;
    TemplateHSMState_t nextstate;

    switch (CurrentState)
    {
    case Init:
        /* code */

        Transition = true;
        InputEvent = NO_EVENT;
        nextstate = Hstate;
        break;
    
    case Hstate:
        // code
        break;
    default:
        // code
        break;
    }
    if(Transition){
        RunTemplateHSM(EXIT_EVENT);
        CurrentState = nextstate;
        RunTemplateHSM(ENTRY_EVENT);
    }

    return InputEvent;
 }
