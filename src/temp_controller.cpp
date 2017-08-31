/*
 * temp_controller.cpp
 *
 *  Created on: 25 Aug 2017
 *      Author: christo
 */

#include "temp_controller.h"
#include "util/delay.h"

#define SHOW_THRESHHOLD     3
#define SHOW_COUNT          1
#define SHOW_COUNT_TIMEOUT  20
#define TEMP_SET_LOW        10
#define TEMP_SET_TOP        40
#define TEMP_SET_STEP       5   //Must be able to divide into difference => (TOP-LOW)/n


uint8_t setTemp = TEMP_SET_LOW;


void cTempController::showTemp()
{
    if (mTempControllerState == TC_SHOW_TEMP)
        mTempControllerState = TC_SHOW_HIGH;
}

uint8_t cTempController::setMode()
{
    if (!mBusy)
        return 0;

    if (setTemp >= TEMP_SET_TOP)
        setTemp = TEMP_SET_LOW;
    else
        setTemp += TEMP_SET_STEP;

    mDisplayController->updateTextNumber(setTemp);
    return 1;
}

cTempController::cTempController(cTemp *temp, cDisplayController *displayController)
{
    mTemp = temp;
    mDisplayController = displayController;;
    mCurrTemp = 0;
    mTempControllerState = TC_SHOW_TEMP;
    mBusy = false;
}

cTempController::~cTempController()
{
    // TODO Auto-generated destructor stub
}


void cTempController::btnShortPress()
{
    if (setMode())
        return;

    showTemp();
}

void cTempController::btnLongPress()
{
    mBusy = true;
    switch (mTempControllerState) {
        case TC_SET_HIGH:
            if (setTemp != mTemp->get_highValue())
                mTemp->set_highValue(setTemp);

            mTempControllerState = TC_SET_LOW;
            break;
        case TC_SET_LOW:
            if (setTemp != mTemp->get_lowValue())
                mTemp->set_lowValue(setTemp);

            mTempControllerState = TC_SHOW_TEMP;
            mBusy = false;
            break;
        default:
            SevenSegment.setState(SEGMENT_SET);
            _delay_ms(1000);

            mTempControllerState = TC_SET_HIGH;
            break;
    }
}

void cTempController::run()
{
    mCurrTemp = mTemp->getLastTemp();

    switch (mTempControllerState)
        {
        case TC_SHOW_TEMP:
        {
            mDisplayController->shownumber(mTemp->getLastTemp());
            //TODO show temp

        }break;
        case TC_SHOW_HIGH:
            if (mDisplayController->showTextNumber(SEGMENT_HI, mTemp->get_highValue(), SHOW_COUNT))
                mTempControllerState = TC_SHOW_LOW;
            break;
        case TC_SHOW_LOW:
            if (mDisplayController->showTextNumber(SEGMENT_LO, mTemp->get_lowValue(), SHOW_COUNT))
                mTempControllerState = TC_SHOW_TEMP;
            break;
        case TC_SET_HIGH:
            if (mDisplayController->showTextNumber(SEGMENT_HI, setTemp, SHOW_COUNT_TIMEOUT))
                mTempControllerState = TC_SHOW_TEMP;
            break;
        case TC_SET_LOW:
            if (mDisplayController->showTextNumber(SEGMENT_LO, setTemp, SHOW_COUNT_TIMEOUT))
                mTempControllerState = TC_SHOW_TEMP;
        }

}


