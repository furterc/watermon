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
public:
    cTemp();
    uint16_t getVal();
    double adc_getResistance();
    double adc_getTemp();

    virtual ~cTemp();


};

#endif /* SRC_TEMP_H_ */
