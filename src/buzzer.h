/*
 * buzzer.h
 *
 *  Created on: 23 Jul 2017
 *      Author: christo
 */

#ifndef SRC_BUZZER_H_
#define SRC_BUZZER_H_

#include <stdint-gcc.h>

class cBuzzer {
    bool mState;
    bool mEnabled;

public:
	cBuzzer();
	virtual ~cBuzzer();

    void init();
    void set();
    void reset();
};

#endif /* SRC_BUZZER_H_ */
