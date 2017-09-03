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

    void set();
    void reset();
public:
	cBuzzer();
	virtual ~cBuzzer();

    void init();
    void enable(bool enabled);
    void run();
};

#endif /* SRC_BUZZER_H_ */
