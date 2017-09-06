/*
 * temp.cpp
 *
 *  Created on: 01 Jul 2017
 *      Author: christo
 */


#include <avr/io.h>
#include <stdint-gcc.h>
#include <math.h>
#include <avr/eeprom.h>
#include "temp.h"

//#define A 1.679277639e-3
//#define B 2.449426116e-4
//#define C 0.4324672627e-7

#define A 1.647367164e-3
#define B 2.474347798e-4
#define C 1.610802365e-7

#define VIN 5
#define RES1 220


#define EEPROM_LO_VALUE 0
#define EEPROM_HI_VALUE 1

cTemp::cTemp()
{
    //get stored values
	mLowVal = eeprom_read_byte((uint8_t *)EEPROM_LO_VALUE);
	mHighVal = eeprom_read_byte((uint8_t *)EEPROM_HI_VALUE);

	mLastTemp = 0;
}

void cTemp::set_lowValue(uint8_t low)
{
	mLowVal = low;
	eeprom_write_byte((uint8_t *)EEPROM_LO_VALUE, low);
}

uint8_t cTemp::get_lowValue()
{
	return mLowVal;
}

void cTemp::set_highValue(uint8_t high)
{
	mHighVal = high;
	eeprom_write_byte((uint8_t *)EEPROM_HI_VALUE, high);
}

uint8_t cTemp::get_highValue()
{
	return eeprom_read_byte((uint8_t *)EEPROM_HI_VALUE);
}

uint16_t cTemp::getVal()
{
	/* Select ADC6 as input */
	ADMUX = (1<<MUX0) | (1<<MUX1) | (1<<REFS0);

	uint8_t sampleCount = 0;
	uint32_t samples = 0;

	while(sampleCount++ < 16)
	{
		//Enable the ADC | start conversion | clear the interrupt flag
		ADCSRA = _BV(ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | _BV(ADSC) | _BV(ADIF);

		/* Wait for the ADC to finish sampling */
		while ( !(ADCSRA & (1 << ADIF)));

		samples += ADC;
	}

	sampleCount = 0;
	samples = (samples >> 4);

	/* Return the calculated voltage */
	return samples;
}

double cTemp::adc_getResistance()
{

	/*
	 * Calculate the resistance
	 * R2 = Vout*R1/(Vin-Vout)
	 * Vout = (ADC*Vin)/1024
	 */
	double Vout = (VIN*getVal());
	Vout = Vout/1024;
	double res = ((Vout*RES1)/(VIN-Vout));
	return res;
}

uint16_t cTemp::adc_getTemp()
{
	double lnRes = (log(adc_getResistance()));
	double temp = 1/(A + (B*lnRes) + (C * lnRes * lnRes * lnRes));
	double tempC = temp - 273;
	return (uint16_t)tempC;
}

void cTemp::run()
{
    static uint8_t sampleCount = 0;
    static uint32_t samples = 0;

    if(sampleCount++ < 16)
        samples += adc_getTemp();
    else
    {
        sampleCount = 0;
        samples = (samples >> 4);
        mLastTemp = samples;
        samples = 0;
    }
}

uint8_t cTemp::getLastTemp()
{
    return mLastTemp;
}

bool cTemp::checkHiLo()
{
    if (mLastTemp > mHighVal)
        return false;

    if (mLastTemp < mLowVal)
        return false;

    return true;
}

cTemp::~cTemp()
{

}
