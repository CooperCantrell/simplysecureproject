/*
 * File: templatehsm.h
 * Author: Anirudh Iyer
 * Date 2-08-2025
 *
 * Template for a hierarchical state machine. This works with the TODO queue and 
 * the FLARE config file 
 *
 */

 #ifndef TEMPLATEHSM_H  // <- This should be changed to your own guard on both
#define TEMPLATEHSM_H  //    of these lines


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
 * @Function InitTemplateHSM(uint16_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author Anirudh Iyer 2/8/2025 */
bool InitTemplateHSM(uint16_t Priority);

/**
 * @Function PostTemplateHSM(Event inpEvent, unint16_t Priority)
 * @param InpEvent - the event (label and data) to be posted to queue
 * @param Priority - the priority of this event in the todo queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to posting to the master queue
 * @author Anirudh Iyer 2/8/2025 */
bool PostTemplateHSM(Event InputEvent, unint16_t Priority);

/**
 * @Function RunTemplateHSM(Event ThisEvent)
 * @param InputEvent - the event (label and data) to be in the state machine
 * @return Event - return event (label and data), should be NO_EVENT 
 * @brief This is a function that you write and rename for your use
 * @author Anirudh Iyer 2/8/2025
 */
Event RunTemplateHSM(Event InputEvent);
 #endif 
