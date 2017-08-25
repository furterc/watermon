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
	enum
	{
		SHOW_TEMP,
		SHOW_HIGH,
		SHOW_LOW,
		SET_HIGH,
		SET_LOW
	}mDisplayState;

	bool mBusy;
	uint8_t showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout);


public:
	cDisplayController();
	virtual ~cDisplayController();

	void show_temp();
	void show_info();
	bool set_mode_busy();
	void enter_set_mode();
	void run();
};


#endif /* SRC_DISPLAY_CONTROLLER_H_ */
