/* 
 * File:   Events.h
 * Author: Cooper Cantrell
 * Created on 2/7/2025
 * 
 * Every state machine or service routine will take an event as a param
 * This file defines what an event is and some common ones 
 */
#ifndef EVENTS_H  
#define EVENTS_H 
/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Master Events config                                                        *
 ******************************************************************************/
// Event enums
typedef enum {
    NONE,
    SERROR,
    INIT,
    ENTRY,
    EXIT,
    TIMEOUT,
    CAP_ON,
    CAP_OFF,
    SET_PASSWORD,
    PING_CLOSE,
    PING_FAR,
    UNLOCK,
    DOOR_OPENED,
    DOOR_CLOSED,
    BUTTON,
} EventLabel;
// static const char *EventNames[] = {
//     "NONE",
//     "SERROR",
//     "INIT",
//     "ENTRY",
//     "EXIT",
//     "TIMEOUT",
//     "CAP_ON",
//     "CAP_OFF",
//     "SET_PASSWORD",
//     "PING_CLOSE",
//     "PING_FAR",
//     "UNLOCK",
//     "DOOR_OPENED",
//     "DOOR_CLOSED",
//     "BUTTON",
// };

 typedef struct 
{
    EventLabel Label;
    uint16_t Data;
}Event;



/*******************************************************************************
 * #DEFINE Special Events                                                                   *
 ******************************************************************************/
#define NO_EVENT (Event){NONE,0}
#define INIT_EVENT (Event){INIT,0}
#define ENTRY_EVENT (Event){ENTRY,0}
#define EXIT_EVENT (Event) {EXIT,0}
#define PRESSED (Event) {CAP_ON,0}
#define UNPRESSED (Event) {CAP_OFF,0}
#endif