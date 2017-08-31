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
	bool mBusy;
public:
	cDisplayController();
	virtual ~cDisplayController();

	uint8_t showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout);
	void updateTextNumber(uint8_t number);;
	void shownumber(uint8_t number);



	bool set_mode_busy();
	void enter_set_mode();
	void run();

};


#endif /* SRC_DISPLAY_CONTROLLER_H_ */
