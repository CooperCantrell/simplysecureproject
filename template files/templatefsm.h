/*
 * File: templatefsh.h
 * Author: Cooper Cantrell
 * Date 2-18-2025
 *
 * Template for a first order state machine This works with the todo queue and 
 * the FLARE config file 
 *
 */

 #ifndef TEMPLATEFSM_H  // <- This should be changed to your own guard on both
#define TEMPLATEFSM_H  //    of these lines


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
// for to work we need to include the following 
// (these are commented out in the template):
// #include <Events.h>
// #include <TODOQueue.h>
// #include <stdbool.h>
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
 * @Function InitTemplateFSM(uint16_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Anirudh Iyer 2/8/2025 */
 bool InitTemplateFSM(uint16_t Priority);

 /**
 * @Function PostTemplateFSM(Event inpEvent, unint16_t Priority)
 * @param InpEvent - the event (label and data) to be posted to queue
 * @param Priority - the priority of this event in the todo queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to posting to the master queue
 * @author Cooper Cantrell 2/8/2025 */
bool PostTemplateFSM(Event InputEvent, unint16_t Priority);

/**
 * @Function RunTemplateFSM(Event ThisEvent)
 * @param InputEvent - the event (label and data) to be in the state machine
 * @return Event - return event (label and data), only exist so I can reuse code for
 * the HSM later, should be NO_EVENT 
 * @brief This is a function that you write and rename for your use
 * @author Cooper Cantrell 2/8/2025
 */
Event RunTemplateFSM(Event InputEvent);
 #endif 