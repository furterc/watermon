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

uint8_t inCount = 0;
uint8_t outCount = 0;
bool showText = false;

cDisplayController::cDisplayController(cSevenSegment *sevenSegment)
{
    mSevenSegment = sevenSegment;
}

/*
 * return 0 - done
 * return 1 - busy
 * return 2 - showText
 * return 3 - showNumber
 * */
uint8_t cDisplayController::showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout)
{
    if(inCount++ > SHOW_THRESHHOLD)
    {
        inCount = 0;

        if (outCount >= timeout && outCount != 1)
        {
            outCount = 0;
            showText = false;
            return 0;
        }

        //toggle text & number
        if(!showText)
        {
            mSevenSegment->setState(state);
            showText = true;
            return 2;
        }
        else
        {
            mSevenSegment->setNumber(number);
            showText = false;
            outCount++;
            return 3;
        }
    }
    return 1;
}

/*
 * return 0 - done
 * return 1 - busy
 * return 2 - showText
 * return 3 - showNumber
 * */
uint8_t cDisplayController::showTextText(segmentState_t state1, segmentState_t state2, uint8_t timeout)
{
    if(inCount++ > SHOW_THRESHHOLD)
    {
        inCount = 0;

        if (outCount >= timeout && outCount != 1)
        {
            outCount = 0;
            showText = false;
            return 0;
        }

        //toggle text & number
        if(!showText)
        {
            mSevenSegment->setState(state1);
            showText = true;
            return 2;
        }
        else
        {
            mSevenSegment->setState(state2);
            showText = false;
            outCount++;
            return 3;
        }
    }
    return 1;
}

void cDisplayController::updateNumber(uint8_t number)
{
    inCount = 0;
    outCount = 0;
    showText = false;
    mSevenSegment->setNumber(number);
}

void cDisplayController::updateText(segmentState_t state)
{
    inCount = 0;
    outCount = 0;
    showText = true;
    mSevenSegment->setState(state);
}

