/*
 * seven_segment.h
 *
 *  Created on: 19 Jul 2017
 *      Author: christo
 */

#ifndef SRC_SEVEN_SEGMENT_H_
#define SRC_SEVEN_SEGMENT_H_

#include <stdint.h>

class cSevenSegment
{
	void sendByte(uint8_t byte);
	uint8_t mDigits[3];
	uint8_t mPoint;

	void putDigit(uint8_t index);

	uint8_t mEnabled;

public:

	typedef enum
	{
		OFF, NUMBER, HI, LO, SET
	} mState;
	mState state;

	cSevenSegment();
	virtual ~cSevenSegment();

	void run();
	void setNumber(uint16_t number);
	void setState(mState st);

};

extern cSevenSegment SevenSegment;

#endif /* SRC_SEVEN_SEGMENT_H_ */
