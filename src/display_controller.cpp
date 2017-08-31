/*
 * display_controller.cpp
 *
 *  Created on: 28 Jul 2017
 *      Author: christo
 */

#include "display_controller.h"

#include <stdlib.h>
#include <avr/delay.h>

#include "terminal.h"
#include "commands.h"

#define SHOW_THRESHHOLD     3
#define SHOW_COUNT		    1
#define SHOW_COUNT_TIMEOUT  20
#define TEMP_SET_LOW        10
#define TEMP_SET_TOP        40
#define TEMP_SET_STEP       5   //Must be able to divide into difference => (TOP-LOW)/n




uint8_t inCount = 0;
uint8_t outCount = 0;
bool showText = false;

uint8_t cDisplayController::showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout)
{
    if(inCount++ > SHOW_THRESHHOLD)
    {
        inCount = 0;

        if (outCount >= timeout)
        {
            outCount = 0;
            showText = false;
            mBusy = false;
            return 1;
        }

        //toggle text & number
        if(!showText)
        {
            SevenSegment.setState(state);
            showText = true;
        }
        else
        {
            SevenSegment.setNumber(number);
            showText = false;
            outCount++;
        }
    }
    return 0;
}

void cDisplayController::shownumber(uint8_t number)
{
    SevenSegment.setNumber(number);
}


void cDisplayController::updateTextNumber(uint8_t number)
{
    inCount = 0;
    outCount = 0;
    showText = true;
    SevenSegment.setNumber(number);
}

cDisplayController::cDisplayController()
{
    mBusy = false;
}

//void cDisplayController::show_info()
//{
////   if (mDisplayState == SHOW_TEMP)
////       mDisplayState = SHOW_HIGH;
//
//   SevenSegment.setState(SEGMENT_HI);
//}

bool cDisplayController::set_mode_busy()
{
    if (!mBusy)
        return false;

//    if (setTemp >= TEMP_SET_TOP)
//        setTemp = TEMP_SET_LOW;
//    else
//        setTemp += TEMP_SET_STEP;
//
//    SevenSegment.setNumber(setTemp);
    inCount = 0;
    outCount = 0;
    showText = false;

    return true;
}

void cDisplayController::enter_set_mode()
{

}


uint8_t segmentCount = 0;
bool tempState = false;
void cDisplayController::run()
{
//	static uint8_t inCount = 0;
//	static uint8_t outCount = 0;
//	static bool showText = false;



}

cDisplayController::~cDisplayController()
{

}

