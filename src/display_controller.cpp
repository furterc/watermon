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

uint8_t cDisplayController::showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout)
{
    if(inCount++ > SHOW_THRESHHOLD)
    {
        inCount = 0;

        if (outCount >= timeout)
        {
            outCount = 0;
            showText = false;
            return 1;
        }

        //toggle text & number
        if(!showText)
        {
            mSevenSegment->setState(state);
            showText = true;
        }
        else
        {
            mSevenSegment->setNumber(number);
            showText = false;
            outCount++;
        }
    }
    return 0;
}

void cDisplayController::shownumber(uint8_t number)
{
    mSevenSegment->setNumber(number);
}

void cDisplayController::updateNumber(uint8_t number)
{
    inCount = 0;
    outCount = 0;
    showText = true;
    mSevenSegment->setNumber(number);
}

void cDisplayController::updateText(segmentState_t state)
{
    inCount = 0;
    outCount = 0;
    showText = true;
    mSevenSegment->setState(state);
}

