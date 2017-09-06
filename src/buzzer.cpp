/*
 * buzzer.cpp
 *
 *  Created on: 23 Jul 2017
 *      Author: christo
 */

#include "buzzer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIM0_OCRN_VAL       51

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

void cBuzzer::init()
{
    DDRD = _BV(PD6);

    OCR0A = TIM0_OCRN_VAL;
    TCCR0A = _BV(WGM01) | _BV(COM0A0);
}

cBuzzer::cBuzzer()
{
    mState = false;
    mEnabled = false;
}

cBuzzer::~cBuzzer()
{

}
