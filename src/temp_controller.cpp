/*
 * temp_controller.cpp
 *
 *  Created on: 25 Aug 2017
 *      Author: christo
 */

#include "temp_controller.h"
#include "util/delay.h"
#include <stdio.h>

#define SHOW_THRESHHOLD     5
#define SHOW_COUNT          1
#define SHOW_COUNT_TIMEOUT  20
#define TEMP_SET_LOW        10
#define TEMP_SET_TOP        40
#define TEMP_SET_STEP       5   //Must be able to divide into difference => (TOP-LOW)/n

#define BEEP_TIME           5

void cTempController::showTemp()
{
    if (mTempControllerState == TC_SHOW_TEMP)
    {
        mTempControllerState = TC_SHOW_HIGH;
        buzzerOnOff(10);
    }
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
    buzzerOnOff(5);

    return 1;
}

bool cTempController::checkWater()
{
    if (WATER_DETECT_PIN &= _BV(WATER_DETECT_PIN_NUM))
        return false;

    return true;
}

cTempController::cTempController(cTemp *temp, cDisplayController *displayController, cBuzzer * buzzer)
{
    mTemp = temp;
    mDisplayController = displayController;
    mBuzzer = buzzer;

    mCurrTemp = 0;
    mSetTemp = TEMP_SET_LOW;

    mTempControllerState = TC_SHOW_TEMP;
    mBusy = false;

    //setup the water detect pin
    WATER_DETECT_PORT &= ~(_BV(WATER_DETECT_PIN_NUM));
}

cTempController::~cTempController()
{

}

void cTempController::buzzerOnOff(uint8_t delayMultiplier)
{
    mBuzzer->set();

    while(delayMultiplier--)
    {
        _delay_ms(BEEP_TIME);
    }

    mBuzzer->reset();
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

        mSetTemp = mTemp->get_lowValue();
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
        _delay_ms(400);

        mSetTemp = mTemp->get_highValue();
        mTempControllerState = TC_SET_HIGH;
        break;
    }
    buzzerOnOff(25);
}

void cTempController::run()
{
    static uint8_t alarmState = 0;

    if (!checkWater())
        alarmState |= 0x02;

    if (alarmState)
    {
        uint8_t buzzState = 0;

        if ((alarmState & 0x02) == 0x02)
        {
            if (checkWater())
                alarmState &= ~0x02;

            buzzState = mDisplayController->showTextText(SEGMENT_ERR, SEGMENT_LVL, SHOW_COUNT_TIMEOUT);
        }
        else if ((alarmState & 0x01) == 0x01)
        {
            if (mTemp->checkHiLo())
                alarmState &= ~0x01;

            buzzState = mDisplayController->showTextNumber(SEGMENT_ERR, mCurrTemp, SHOW_COUNT_TIMEOUT);
        }

        /* Set mode active */
        if (mTempControllerState == TC_SET_HIGH || mTempControllerState == TC_SET_LOW)
        {
            mBuzzer->reset();
        }
        else
        {
            /* Toggle Buzzer */
            if (buzzState == 2)
                mBuzzer->set();
            else if (buzzState == 3)
                mBuzzer->reset();

            /* Disable alarm mode */
            if(!alarmState)
            {
                mBuzzer->reset();
                mTempControllerState = TC_SHOW_TEMP;
            }

            if (mTempControllerState != TC_SET_HIGH || mTempControllerState != TC_SET_LOW)
                return;

        }
    }

    mCurrTemp = mTemp->getLastTemp();

    switch (mTempControllerState)
    {
    case TC_SHOW_TEMP:
    {
        mDisplayController->updateNumber(mTemp->getLastTemp());

        if (!mTemp->checkHiLo())
            alarmState |= 0x01;

    }break;
    case TC_SHOW_HIGH:
        if (!mDisplayController->showTextNumber(SEGMENT_HI, mTemp->get_highValue(), SHOW_COUNT))
            mTempControllerState = TC_SHOW_LOW;
        break;
    case TC_SHOW_LOW:
        if (!mDisplayController->showTextNumber(SEGMENT_LO, mTemp->get_lowValue(), SHOW_COUNT))
            mTempControllerState = TC_SHOW_TEMP;
        break;
    case TC_SET_HIGH:
        alarmState = 0;
        mBuzzer->reset();
        if (!mDisplayController->showTextNumber(SEGMENT_HI, mSetTemp, SHOW_COUNT_TIMEOUT))
            mTempControllerState = TC_SHOW_TEMP;
        break;
    case TC_SET_LOW:
        if (!mDisplayController->showTextNumber(SEGMENT_LO, mSetTemp, SHOW_COUNT_TIMEOUT))
            mTempControllerState = TC_SHOW_TEMP;
    }
}


