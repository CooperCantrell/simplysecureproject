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
    UI_DEVICESETTINGS,
    UI_MANUALSCAN,
    INTERNAL_SCHEDULETIMERINTERRUPT,
    CAP_ON,
    CAP_OFF,
} EventLabel;
static const char *EventNames[] = {
    "NONE",
    "SERROR",
    "INIT",
    "ENTRY",
    "EXIT",
    "TIMEOUT",
    "UI_DEVICESETTINGS",
    "UI_MANUALSCAN",
    "INTERNAL_SCHEDULETIMERINTERRUPT",
    "CAP_ON",
    "CAP_OFF",
};

 typedef struct 
{
    EventLabel Label;
    void* Data;
}Event;



/*******************************************************************************
 * #DEFINE Special Events                                                                   *
 ******************************************************************************/
#define NO_EVENT (Event){NONE,NULL}
#define INIT_EVENT (Event){INIT,NULL}
#define ENTRY_EVENT (Event){ENTRY,NULL}
#define EXIT_EVENT (Event) {EXIT,NULL}

#endif