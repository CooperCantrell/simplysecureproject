/* 
 * File:   TimerPost.h
 * Author: Cooper Cantrell
 * Editor: --
 * Created on 3/6/2025
 * Edited on --
 */
#ifndef TimerPost_H  // <- This should be changed to your own guard on both
#define TimerPost_H //    of these lines

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include <inttypes.h>
#include <Events.h>
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
 * @Function TimerPosting(type Input)
 * @param Input - discription of input 
 * @return what the return value should look like
 * @brief what the function does and why
 * @author YOURNAME, DATE 
 * @edited EDITORNAME, DATE - only if applies
*/
bool ExampleFuncton(uint32_t Time, Event(*PostItem)(Event), uint16_t ID);


#endif 