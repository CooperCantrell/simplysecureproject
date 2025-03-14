
 /*
 * File: TODOQueue.c
 * Author: Cooper Cantrell
 * Modified: --
 * This defines the functions that are created in TODOQueue.h
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include "TODOQueue.h"
#include "timers.h"
#include <stdlib.h>
#include <assert.h>
#include <config.h>
#include <TimerPost.h>
#include <buttons.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/

typedef struct NodeObj* Node;
// private NodeObj type
typedef struct NodeObj{ 
    uint32_t Priority;
    TODOItem data;
    Node next;
} NodeObj;
typedef struct TODOListobj {
    uint16_t Size;
    Node Head;
    Node Free;
    bool full;
}TODOListobj;
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
TODOList MasterQueue;
bool (*StateMachineINIT[])(uint16_t) = SM_INIT;
uint8_t LastState;
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/
// Constructors-Destructors
Node newNode(TODOItem data){
   Node N = malloc(sizeof(NodeObj));
   assert( N!=NULL );
   N->data = data;
   N->next = NULL;
   return(N);
}

void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}
 // Inserts node B after Node A in A list
void InsertAfter(Node A, Node B){
    B->next = A->next;
    A->next = B;
}
/*******************************************************************************
 * PUBLIC FUNCTIONS FUNCTIONS                                                  *
 ******************************************************************************/

// define functions in the .h file

/**
 * @Function TODOQueue_init(uint16_t Size);
 * @param  uint16_t how large the circular buffer will be 
 * @return TODOList - A pointer to a TODOList obj
 * @brief This function is responsible for initializing the todo Queue
 * @author Cooper Cantrell, 1/28/2025 
 * @edited --
*/
TODOList TODOQueue_init(uint16_t Size){
    TODOList out = malloc(sizeof(struct TODOListobj));
    out->Size = Size;
    // Trivial case, cannot create obj 
    if (Size<1){
        out->full = true;
        out->Head = (Node) NULL;    
        return out;        
    }
    Node Current = newNode(EMPTY_ITEM);
    Node Last;
    out->Free = Current;
    for (uint16_t i = 0; i < Size-1; i++)
    {
        Last = Current;
        Current = newNode(EMPTY_ITEM);
        Last->next = Current;
    }
    // set the output
    out->Head = NULL;
    out->full = false;
    BUTTONS_Init();
    LastState =  buttons_state();
    return out;
}


/**
 * @Function EnQueue(uint16_t Priority, uint16_t item)
 * @param Priority Priority of the item in the queue, the higher the number the 
 * closer to the front of the list 
 * @param Item a TODOItem that contains the data for the queue
 * @return bool True if successful False if not
 * @brief This function adds an item to the running TODO queue with the proper 
 * priority to be processed as the RunQueue function gets to it
 * @author Cooper Cantrell, 1/27/2025 
 * @edited --
*/
bool EnQueue(TODOList Queue, uint32_t Priority, TODOItem Item){
    // if the list is full but is higher priority than something that exists on the list, 
    // the lowest priority will be removed and new item will be added in its proper place
    // NOT YET IMPLEMENTED AS OF NOW IF FULL WONT ADD
    if (Queue->full)
    {
        // will add checks and sorting later
        return false;        
    }
    else
    {
        // take a free node off
        Node Temp, Current, Prev;
        Temp = Queue->Free;
        Queue->Free = Queue->Free->next;
        // add data to temp
        Temp->data = Item;
        Temp->Priority = Priority;
        Temp->next = NULL;
        // add the free node to the head
        if (Queue->Head == NULL)
        {
            Queue->Head=Temp;
        }
        else if(Priority > Queue->Head->Priority){
            Temp->next = Queue->Head;
            Queue->Head = Temp;
        }
        else{
            Current = Queue->Head;
            Prev = Current;
            while (Priority <= Current->Priority)
            {
                if (Current->next == NULL)
                {
                    Prev = Current;
                    break;
                }
                Prev = Current;
                Current = Current->next;  

            }
            InsertAfter(Prev,Temp);
        }
        
        if (Queue->Free == NULL)
        {
            Queue->full = true;
        }
        return true;
    }
    
}

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
TODOItem DeQueue(TODOList Queue){
    if (Queue->Head == NULL)
    {
        return EMPTY_ITEM;
    }
    else
    {
        Node Temp = Queue->Head;
        Queue->Head = Queue->Head->next;
        Temp->next = Queue->Free;
        Queue->Free = Temp;
        Queue->full = false;
        return Temp->data;
    }
    
    
}

/**
 * @Function IsEmpty(TODOList Queue)
 * @param TODOList Queue the queue you wish to check if empty  
 * @return bool True if empty False otherwise
 * @brief this function checks if the todo list is empty or not 
 * @author Cooper Cantrell, 2/1/2025 
 * @edited --
*/ 
bool IsEmpty(TODOList Queue){
    return Queue->Head == NULL;
}

/**
 * @Function Execute(TODOList Queue)
 * @param TODOList Queue the queue you wish to execute the next function in line from  
 * @return Event
 * @brief This function will run the next item in the todo queue, if the queue is empty
 * no errors will happen as it will check prior to running the function
 * @author Cooper Cantrell, 2/1/2025 
 * @edited --
*/ 
Event Execute(TODOList Queue){
    if (!IsEmpty(Queue))
    {
        TODOItem Stuff = DeQueue(Queue);
        return Stuff.Func(Stuff.Input);
    }
    return NO_EVENT;
}

/**
 * @Function RunQueue(void)
 * @param none 
 * @return none
 * @brief This function is responsible for initializing and running a perpetual 'Main Queue'
 * check on a queue that contains a list of functions that need to be ran as well 
 * as there parameters
 * @author Cooper Cantrell, 1/27/2025 
 * @edited --
*/
void RunQueue(void){
    // Init the Queue to the size in FLARECONFIG
    MasterQueue = TODOQueue_init(TODO_SIZE);
    BUTTONS_Init();
    // Init all needed state machines (IN PROGRESS)
    for (size_t i = 0; i < ARRAY_SIZE(StateMachineINIT); i++)
    {
        StateMachineINIT[i](0);
    }
    
    // Run forever completing the items in the queue
    while (true)
    {
        if(Execute(MasterQueue).Label == SERROR){
            break;
        }
        runtimer();
        // change in button state
        if (LastState ^ buttons_state())
        {
            LastState = buttons_state();
            PostSimplyFSM((Event){BUTTON,&LastState}, 4);
        }
        
    }
    
}

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
bool EnQueueMaster(uint32_t Priority, TODOItem item){
    return EnQueue(MasterQueue,Priority,item);
}

/**
 * @Function TODOQueue_exit(TODOList List);
 * @param  TODOList The TODOList that will be freed in mem 
 * @return void 
 * @brief This function is responsible for freeing the memory in the TODOQueue
 * @author Cooper Cantrell, 1/28/2025 
 * @edited --
*/
void TODOQueue_exit(TODOList* PT){
    TODOList List = *PT;
    Node CurrentNode = List->Head;
    Node LastNode;
    // Free the head
    while (CurrentNode != NULL)
    {
        LastNode=CurrentNode;
        CurrentNode = CurrentNode->next;
        freeNode(&LastNode);
    }
    // Free the free list 
    CurrentNode = List->Free;
    while (CurrentNode != NULL)
    {
        LastNode=CurrentNode;
        CurrentNode = CurrentNode->next;
        freeNode(&LastNode);
    }
    free (List);
    *PT = NULL;
}


// only for 167
uint32_t ReadPriorty(TODOList Queue){
    if (IsEmpty(Queue))
    {
        return -1;
    }
    else
    {
        return Queue->Head->Priority;
    }
    
}
void* GetData (TODOList Queue){
    return Queue->Head->data.Input.Data;
}
#ifdef TODOQUEUE_TEST
//tests go here
#include <stdio.h>
void Speak(uint32_t a ,void* b){
    printf("Hello\n");
}

int main(void){
    // Testing the creation of a TODOQueue
    TODOList A = TODOQueue_init(4);
    // Testing the enqueue function
    TODOItem F;
    F.Data = NULL;
    F.Label = 0;
    F.Func = Speak;
    EnQueue(A,3,F);
    EnQueue(A,30,F);
    //EnQueue(A,5,EMPTY_ITEM);
    EnQueue(A,1,F);
    printf("BEFORE\n");
    Execute(A);
    printf("AFTER\n");
    //Execute(A);
    EnQueue(A,11,F);
    TODOQueue_exit(A);

}
#endif