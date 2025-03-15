/* 
 * File:   TODOQueue.h
 * Author: Cooper Cantrell
 * This header inits a TODOQueue data structure this structure is a 
 * linked list circular buffer that will allow Enqueue with diffent prioritys
 * This will be the backbone of the ISR interacting with the state machine 
 * or other more complex functions 
 * Editor: --
 * Created on 1/27/2025
 * Edited on 2/1/2025
 * Edited on 2/6/2025
 */
#ifndef TODOQUEUE_H  
#define TODOQUEUE_H 

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <Events.h>
#include <config.h>
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define EMPTY_ITEM (TODOItem) {0,NO_EVENT} 
#define TODO_SIZE 64
/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/

typedef struct
{
    Event (*Func)(Event);
    Event Input;
}TODOItem;
typedef struct TODOListobj* TODOList;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/


/**
 * @Function TODOQueue_init(uint16_t Size);
 * @param  uint16_t how large the circular buffer will be 
 * @return TODOList - A pointer to a TODOList obj
 * @brief This function is responsible for initializing the todo Queue
 * @author Cooper Cantrell, 1/28/2025 
 * @edited --
*/
TODOList TODOQueue_init(uint16_t Size);


/**
 * @Function EnQueue(TODOList Queue, uint16_t Priority, uint16_t item)
 * @param Queue, The Queue you want to add to, to add to the master queue
 * use the EnQueueMaster function instead.
 * @param Priority Priority of the item in the queue, the higher the number the 
 * closer to the front of the list 
 * @param Item a TODOItem that contains the data for the queue
 * @return bool True if successful False if not
 * @brief This function adds an item to the running TODO queue with the proper 
 * priority to be processed as the RunQueue function gets to it
 * @author Cooper Cantrell, 1/27/2025 
 * @edited --
*/
bool EnQueue(TODOList Queue, uint32_t Priority, TODOItem Item);

/**
 * @Function DeQueue(TODOList Queue)
 * @param TODOList Queue the queue you wish to dequeue from 
 * @return TODOItem
 * @brief This function returns the next item in the todo queue and places it back in the
 * free 'pile' 
 * @note If the list is empty will return EMPTY_ITEM, should check with the isempty function
 * @author Cooper Cantrell, 2/1/2025 
 * @edited --
*/ 
TODOItem DeQueue(TODOList Queue);

/**
 * @Function IsEmpty(TODOList Queue)
 * @param TODOList Queue the queue you wish to check if empty  
 * @return bool True if empty False otherwise
 * @brief this function checks if the todo list is empty or not 
 * @author Cooper Cantrell, 2/1/2025 
 * @edited --
*/ 
bool IsEmpty(TODOList Queue);

/**
 * @Function Execute(TODOList Queue)
 * @param TODOList Queue the queue you wish to execute the next function in line from  
 * @return Event
 * @brief This function will run the next item in the todo queue, if the queue is empty
 * no errors will happen as it will check prior to running the function
 * @author Cooper Cantrell, 2/1/2025 
 * @edited --
*/ 
Event Execute(TODOList Queue);

/**
 * @Function RunQueue(void)
 * @param none 
 * @return none
 * @brief This function is responsible for initializing and running a perpetual 
 * check on a queue that contains a list of functions that need to be ran as well 
 * as there parameters
 * @author Cooper Cantrell, 1/27/2025 
 * @edited --
*/
void RunQueue(void);


/**
 * @Function EnQueueMaster(uint16_t Priority, TODOItem item)
 * @param Priority Priority of the item in the queue, the higher the number the 
 * closer to the front of the list 
 * @param Item a TODOItem that contains the data for the queue
 * @return bool True if successful False if not
 * @brief This function adds an item to the Master Queue in main
 * @author Cooper Cantrell, 2/6/2025 
 * @edited --
*/
bool EnQueueMaster(uint32_t Priority, TODOItem item);
/**
 * @Function TODOQueue_exit(TODOList List);
 * @param  TODOList The TODOList that will be freed in mem 
 * @return void 
 * @brief This function is responsible for freeing the memory in the TODOQueue
 * @author Cooper Cantrell, 1/28/2025 
 * @edited --
*/
void TODOQueue_exit(TODOList* List);

// only for 167 ----
uint32_t ReadPriorty(TODOList Queue);
#endif 