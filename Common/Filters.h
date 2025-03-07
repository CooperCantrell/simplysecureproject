/* 
 * File:   Filters.h
 * Author: Cooper Cantrell
 * Created on 1/13/2025
 */
#ifndef FILTERS_H
#define FILTERS_H
/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/
#include <stdbool.h>
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
/*
 * @Function MovingAvgFIT(int Data, unsigned char ID)
 * @param Data - the raw int data that will be smoothed via this filter
 * @return an int that is the result of the moving average filter
 * @brief is an FIT filter that takes the last 256 inputs and averages them out for smoothness
 * @author Cooper Cantrell, 1/13/2025 
*/
int MovingAvgFIT(int Data, unsigned char ID);
/*
 * @Function HystFilter256(int Data,int Thresh, int Tol, short ID)
 * @param Data - the data that will be compared in this filter
 * @param Thresh - the Threshold when the data changes shape
 * @param Tol - the spread of the hysteresis filter
 * @param ID - This functon can be used for up to 256 differnt hysteresis latches, ID selects the one that will be used at this time
 * @return bool - a TRUE or FALSE depending on if the component of the slected ID is ON or OFF given the data thresh and TOL
 * @brief This is a pre set up hysteresis filter that can pe used and pluged into many projects 
 * @author Cooper Cantrell, 1/14/2025 
*/
bool HystFilter256(int Data,int Thresh, int Tol, unsigned char ID);
#endif