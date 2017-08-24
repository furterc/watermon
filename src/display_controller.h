/*
 * display_controller.h
 *
 *  Created on: 28 Jul 2017
 *      Author: christo
 */

#ifndef SRC_DISPLAY_CONTROLLER_H_
#define SRC_DISPLAY_CONTROLLER_H_

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




public:
	cDisplayController();
	virtual ~cDisplayController();

	void show_temp();
	void show_info();
	void run();
};

extern cDisplayController DisplayController;

#endif /* SRC_DISPLAY_CONTROLLER_H_ */
