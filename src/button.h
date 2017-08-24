/*
 * button.h
 *
 *  Created on: 27 Jul 2017
 *      Author: christo
 */

#ifndef SRC_BUTTON_H_
#define SRC_BUTTON_H_

#include <avr/io.h>
#include <stdbool.h>

#define BUTTON_DDR  	DDRD
#define BUTTON_PORT 	PORTD
#define BUTTON_PIN  	PIND
#define BUTTON_PIN_NUM 	PD4


class cButton
{
	void (*callback)(bool, uint8_t) = 0;
	bool mState;
public:
	cButton();
	virtual ~cButton();

	void setCB(void (*cb)(bool, uint8_t));
	void run();
};

#endif /* SRC_BUTTON_H_ */
