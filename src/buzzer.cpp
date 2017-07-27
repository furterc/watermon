/*
 * buzzer.cpp
 *
 *  Created on: 23 Jul 2017
 *      Author: christo
 */

#include "buzzer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

cBuzzer::cBuzzer()
{
	DDRD = _BV(PD3);
	PORTD &= ~_BV(PD3);

	TIMSK0 = _BV(TOIE0);
	TCNT0 = 0x68;
	TCCR0B = 0x03;

}

cBuzzer::~cBuzzer()
{

}

ISR(TIMER0_OVF_vect)
{
//	TCNT0 = 125;
//	PORTD ^= _BV(PD3);
}


