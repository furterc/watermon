/*
 * button.cpp
 *
 *  Created on: 27 Jul 2017
 *      Author: christo
 */

#include "button.h"
#include "terminal.h"

cButton::cButton()
{

	//set the button as input
	BUTTON_DDR &= ~(_BV(BUTTON_PIN_NUM));
	mState = (BUTTON_PIN & _BV(BUTTON_PIN_NUM));
}

void cButton::setCB(void (*cb)(bool, uint8_t))
{
	if(cb)
		callback = cb;
}

uint8_t count = 0;

void cButton::run()
{
	if (!callback)
		return;

	bool tmpState = (BUTTON_PIN & _BV(BUTTON_PIN_NUM));

	if (mState == tmpState)
	{
		//button not pressed
		if (tmpState)
			return;

		callback(mState, count++);
		return;
	}

	count = 0;
	mState = tmpState;
	callback(mState, 0);
}

cButton::~cButton()
{

}

