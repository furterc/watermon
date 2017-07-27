/*
 * temp.cpp
 *
 *  Created on: 01 Jul 2017
 *      Author: christo
 */


#include <avr/io.h>
#include <stdio.h>
#include <stdint-gcc.h>
#include <math.h>
#include "temp.h"


cTemp::cTemp()
{
    // TODO Auto-generated constructor stub

}

uint16_t cTemp::getVal()
{
    /* Select ADC6 as input */
    ADMUX = (1<<MUX0) | (1<<MUX1) | (1<<REFS0);

    //Enable the ADC | start conversion | clear the interrupt flag
    ADCSRA = _BV(ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | _BV(ADSC) | _BV(ADIF);

    /* Wait for the ADC to finish sampling */
    while ( !(ADCSRA & (1 << ADIF)));

    /* Return the calculated voltage */
    return ADC;
}

#define A 1.679277639e-3
#define B 2.449426116e-4
#define C 0.04324672627e-7

#define VIN 5
#define RES1 470
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

double cTemp::adc_getTemp()
{
    double lnRes = (log(adc_getResistance()));
    double temp = 1/(A + (B + (C * lnRes * lnRes))*lnRes);
    double tempC = temp - 273;
    return tempC;
}

cTemp::~cTemp()
{
    // TODO Auto-generated destructor stub
}

