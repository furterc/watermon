/*
 * seven_segment.cpp
 *
 *  Created on: 19 Jul 2017
 *      Author: christo
 */

#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "terminal.h"
#include "seven_segment.h"

#define SEVEN_SEGMENT_CHARACTERS    17

const uint8_t segmentValues[] PROGMEM =
{
		0b11100111,	//0
		0b10000100,	//1
		0b11010011,	//2
		0b11010101,	//3
		0b10110100,	//4
		0b01110101, //5
		0b01110111, //6
		0b11000100, //7
		0b11110111, //8
		0b11110101, //9
		0b00010000,	//- (10)
		0b10110110, //H	(11)
		0b00000010, //i	(12)
		0b00100011, //L (13)
		0b00010111, //o (14)
		0b01110011, //E (15)
		0b00110011, //t (16)
		0b00010010, //r (17)
};

cSevenSegment::cSevenSegment() {
	mPoint = 0;
	memset(mDigits, 0, 3);
	mEnabled = 0;

	DDRB = 0xFF;
	PORTB = 0x00;

	TIMSK1 = _BV(TOIE1);
	TCCR1B = 0x01;
	state = SEGMENT_OFF;
}

void cSevenSegment::sendByte(uint8_t byte)
{
	for(uint8_t k = 0; k < 8; k++)
	{
		//set serial data bit to correct value
		if(byte & (1 << k))
			PORTB |= _BV(PB0);
		else
			PORTB &= ~_BV(PB0);

		//clock set bit into serial register
		PORTB |= _BV(PB1);
		PORTB &= ~_BV(PB1);
	}

	//latch register to output pins
	PORTB &= ~_BV(PB2);
	PORTB |= _BV(PB2);
}

void cSevenSegment::putDigit(uint8_t index)
{
	if(index > SEVEN_SEGMENT_CHARACTERS)
		return;

	sendByte(pgm_read_byte(&segmentValues[index]));
}

void cSevenSegment::run()
{
	static uint8_t k = 0;

	//reset port
	PORTB &= 0x07;

	//put digit
	putDigit(mDigits[k]);

	//clock and latch
	if((mEnabled & _BV(k)) == _BV(k))
		PORTB |= _BV(k+3);

	if(k++ >= 2)
		k = 0;
}

void cSevenSegment::setNumber(uint16_t number)
{
	if (number > 999)
		return;

	memset(mDigits, 0xFF, 3);

	uint8_t numberCnt;

	if(number < 10)
		numberCnt = 1;
	else if(number < 100)
		numberCnt = 2;
	else if(number < 1000)
		numberCnt = 3;

	switch (numberCnt)
	{
	case 1:
		mDigits[2] = number;
		mEnabled = 0x4;
		break;
	case 2:
		mDigits[2] = number % 10;
		mDigits[1] = number / 10;
		mEnabled = 0x6;
		break;
	case 3:
		mDigits[2] =  number % 10;
		mDigits[1] = (number / 10) % 10;
		mDigits[0] = (number / 100);
		mEnabled = 0x7;
		break;
	default:
		break;
	}

	state = SEGMENT_NUMBER;
}

void cSevenSegment::setState(segmentState_t st)
{
	memset(mDigits, 0xFF, 3);
	state = st;
	switch (state)
	{
	case SEGMENT_OFF:
		mEnabled = 0;
		break;
	case SEGMENT_HI:
		mDigits[0] = 11;
		mDigits[1] = 12;
		mEnabled = 0x3;
		break;
	case SEGMENT_LO:
		mDigits[0] = 13;
		mDigits[1] = 14;
		mEnabled = 0x3;
		break;
	case SEGMENT_SET:
	    mDigits[0] = 5;
	    mDigits[1] = 15;
	    mDigits[2] = 16;
	    mEnabled = 0x7;
	    break;
	default:
		mEnabled = 0;

	}
}

cSevenSegment::~cSevenSegment() {
	// TODO Auto-generated destructor stub
}


