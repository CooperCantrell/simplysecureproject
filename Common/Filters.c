/* 
* File: Filters.c
 * Author: Cooper Cantrell
 * Modified: 1/13/2025
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include "Filters.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define FILTERS_TEST
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
bool HStatus[256] = {false};
int DataArray[32][265];
/*******************************************************************************
 * PRIVATE FUNCTIONS/CLASSES                                                   *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                   *
 ******************************************************************************/

// define functions in the .h file



/*
 * @Function MovingAvgFIT(int Data, unsigned char ID)
 * @param Input - discription of input 
 * @return an int that is the result of the moving average filter at the ID
 * @brief is an FIT filter that takes the last 256 inputs and averages them out for smoothness
 * @author Cooper Cantrell, 1/13/2025 
*/
int MovingAvgFIT(int Data, unsigned char ID){
    static short Index = 0;
    int sum = 0;
    ID = ID%32;
    DataArray[ID][Index] = Data;
    Index = (Index + 1) % 256;
    for(short i = 0; i < 256; i++){
        sum += DataArray[ID][i];
    }
    return sum/256;
}

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
bool HystFilter256(int Data,int Thresh, int Tol, unsigned char ID){
    if (HStatus[ID] && (Data<(Thresh-Tol)))
    {
        HStatus[ID] = false;
    }
    else if (!HStatus[ID] && (Data>(Thresh+Tol)))
    {
        HStatus[ID] = true;
    }
    return HStatus[ID];
}

#ifdef FILTERS_TEST
#include <assert.h>
//tests go here
int dataout;
int main(void){
    // test MovingAvgFIT
    // CHANGE TO FUNCTON TESTS ARE OUTDATED
    // assert(MovingAvgFIT(0) == 0);
    // assert(MovingAvgFIT(8) == 1);
    // assert(MovingAvgFIT(8) == 2);
    // assert(MovingAvgFIT(16) == 4);
    // MovingAvgFIT(0);
    // MovingAvgFIT(0);
    // MovingAvgFIT(0);
    // MovingAvgFIT(0);
    // assert(MovingAvgFIT(0) == 4);
    // assert(MovingAvgFIT(0) == 3);
    // test HystFilter256
    for (size_t i = 0; i < 256; i++)
    {
        assert(!HStatus[i]);
    }
    assert(!HystFilter256(100,100,1,0));
    assert(HystFilter256(102,100,1,0));
    assert(HystFilter256(102,100,1,13));
    assert(HystFilter256(100,100,1,13));
    assert(HystFilter256(99,100,1,13));
    assert(!HystFilter256(98,100,1,13));
    return 1;

};
#endif