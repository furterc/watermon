/*
 * display_controller.h
 *
 *  Created on: 28 Jul 2017
 *      Author: christo
 */

#ifndef SRC_DISPLAY_CONTROLLER_H_
#define SRC_DISPLAY_CONTROLLER_H_

#include "seven_segment.h"

class cDisplayController
{
    cSevenSegment *mSevenSegment;
public:
	cDisplayController(cSevenSegment *sevenSegment);

	uint8_t showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout);
	uint8_t showTextText(segmentState_t state1, segmentState_t state2, uint8_t timeout);
	void updateNumber(uint8_t number);
	void updateText(segmentState_t state);
};


#endif /* SRC_DISPLAY_CONTROLLER_H_ */
