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




void cTempController::showTemp()
{
    if (mTempControllerState == TC_SHOW_TEMP)
        mTempControllerState = TC_SHOW_HIGH;
}

uint8_t cTempController::setMode()
{
    if (!mBusy)
        return 0;

    if (mSetTemp >= TEMP_SET_TOP)
        mSetTemp = TEMP_SET_LOW;
    else
        mSetTemp += TEMP_SET_STEP;

    mDisplayController->updateNumber(mSetTemp);
    return 1;
}

bool cTempController::checkWater()
{
    if (WATER_DETECT_PORT &= _BV(WATER_DETECT_PIN))
        return true;

    return false;
}

cTempController::cTempController(cTemp *temp, cDisplayController *displayController)
{
    mTemp = temp;
    mDisplayController = displayController;
    mCurrTemp = 0;
    mSetTemp = TEMP_SET_LOW;
    mTempControllerState = TC_SHOW_TEMP;
    mBusy = false;

    //setup the water detect pin
    WATER_DETECT_PORT |= _BV(WATER_DETECT_PIN);
}

cTempController::~cTempController()
{

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
            if (mSetTemp != mTemp->get_highValue())
                mTemp->set_highValue(mSetTemp);

            mTempControllerState = TC_SET_LOW;
            break;
        case TC_SET_LOW:
            if (mSetTemp != mTemp->get_lowValue())
                mTemp->set_lowValue(mSetTemp);

            mTempControllerState = TC_SHOW_TEMP;
            mBusy = false;
            break;
        default:
            mDisplayController->updateText(SEGMENT_SET);
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
            if (mDisplayController->showTextNumber(SEGMENT_HI, mSetTemp, SHOW_COUNT_TIMEOUT))
                mTempControllerState = TC_SHOW_TEMP;
            break;
        case TC_SET_LOW:
            if (mDisplayController->showTextNumber(SEGMENT_LO, mSetTemp, SHOW_COUNT_TIMEOUT))
                mTempControllerState = TC_SHOW_TEMP;
        }

}


