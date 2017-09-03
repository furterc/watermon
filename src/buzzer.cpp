/*
 * buzzer.cpp
 *
 *  Created on: 23 Jul 2017
 *      Author: christo
 */

#include "buzzer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define TIM0_OCRN_VAL       51
#define BUZZER_TOGGLE_TIME  5 /* 0.1 mS */

void cBuzzer::set()
{
    TCCR0B |= 0x03;
    mState = true;
}

void cBuzzer::reset()
{
    TCCR0B &= ~0x03;
    mState = false;
}

void cBuzzer::run()
{
    if (!mEnabled)
        return;


    static uint8_t count = 0;

    if (count++ < BUZZER_TOGGLE_TIME)
        return;

    count = 0;
    if (mState)
        reset();
    else
        set();
}

void cBuzzer::init()
{
    printf("Buzzer init()\n");
    DDRD = _BV(PD6);

    OCR0A = TIM0_OCRN_VAL;
    TCCR0A = _BV(WGM01) | _BV(COM0A0);
}

void cBuzzer::enable(bool enabled)
{
    if (enabled == mEnabled)
        return;

    mEnabled = enabled;

    if (!mEnabled)
        reset();
}

cBuzzer::cBuzzer()
{
    mState = false;
    mEnabled = false;
}

cBuzzer::~cBuzzer()
{

}
