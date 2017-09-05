/*
 * temp.h
 *
 *  Created on: 01 Jul 2017
 *      Author: christo
 */

#ifndef SRC_TEMP_H_
#define SRC_TEMP_H_

#include <stdint-gcc.h>

class cTemp
{
	uint8_t mLowVal;
	uint8_t mHighVal;
	uint8_t mLastTemp;

	uint16_t getVal();
	double adc_getResistance();
	uint16_t adc_getTemp();

public:
    cTemp();

    void set_lowValue(uint8_t temp);
    uint8_t get_lowValue();
    void set_highValue(uint8_t temp);
    uint8_t get_highValue();



    void run();
    uint8_t getLastTemp();
    bool checkHiLo();

    virtual ~cTemp();
};

#endif /* SRC_TEMP_H_ */
