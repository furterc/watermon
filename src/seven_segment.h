/*
 * seven_segment.h
 *
 *  Created on: 19 Jul 2017
 *      Author: christo
 */

#ifndef SRC_SEVEN_SEGMENT_H_
#define SRC_SEVEN_SEGMENT_H_

#include <stdint.h>

typedef enum
    {
    SEGMENT_OFF,
    SEGMENT_NUMBER,
    SEGMENT_HI,
    SEGMENT_LO,
    SEGMENT_SET,
    SEGMENT_ERR,
    SEGMENT_LVL
    } segmentState_t;

class cSevenSegment
{
	void sendByte(uint8_t byte);
	uint8_t mDigits[3];
	uint8_t mPoint;

	void putDigit(uint8_t index);

	uint8_t mEnabled;
	segmentState_t state;

public:
	cSevenSegment();
	virtual ~cSevenSegment();

	void run();
	void setNumber(uint16_t number);
	void setState(segmentState_t st);
};

#endif /* SRC_SEVEN_SEGMENT_H_ */
