/*
 * display_controller.cpp
 *
 *  Created on: 28 Jul 2017
 *      Author: christo
 */

#include "display_controller.h"

#include <stdlib.h>
#include <avr/delay.h>


#include "temp.h"
#include "terminal.h"
#include "commands.h"

#define SHOW_THRESHHOLD     3
#define SHOW_COUNT		    1
#define SHOW_COUNT_TIMEOUT  20
#define TEMP_SET_LOW        10
#define TEMP_SET_TOP        40
#define TEMP_SET_STEP       5   //Must be able to divide into difference => (TOP-LOW)/n


uint8_t setTemp = TEMP_SET_LOW;

cTemp temp = cTemp();

uint8_t inCount = 0;
uint8_t outCount = 0;
bool showText = false;

uint8_t cDisplayController::showTextNumber(segmentState_t state, uint8_t number, uint8_t timeout)
{
    if(inCount++ > SHOW_THRESHHOLD)
    {
        inCount = 0;

        if (outCount >= timeout)
        {
            outCount = 0;
            showText = false;
            mBusy = false;
            return 1;
        }

        if(!showText)
        {
            SevenSegment.setState(state);
            showText = true;
        }
        else
        {
            SevenSegment.setNumber(number);
            showText = false;
            outCount++;
        }
    }
    return 0;
}


cDisplayController::cDisplayController()
{
	mDisplayState = SHOW_TEMP;
	mBusy = false;
}

void cDisplayController::show_temp()
{
	mDisplayState = SHOW_TEMP;
}

void cDisplayController::show_info()
{
   if (mDisplayState == SHOW_TEMP)
       mDisplayState = SHOW_HIGH;

   SevenSegment.setState(SEGMENT_HI);
}

bool cDisplayController::set_mode_busy()
{
    if (!mBusy)
        return false;

    if (setTemp >= TEMP_SET_TOP)
        setTemp = TEMP_SET_LOW;
    else
        setTemp += TEMP_SET_STEP;

    SevenSegment.setNumber(setTemp);
    inCount = 0;
    outCount = 0;
    showText = false;

    return true;
}

void cDisplayController::enter_set_mode()
{
    mBusy = true;
    switch (mDisplayState) {
        case SET_HIGH:
            inCount = 0;
            outCount = 0;
            showText = false;

            if (setTemp != temp.get_highValue())
                temp.set_highValue(setTemp);

            mDisplayState = SET_LOW;
            break;
        case SET_LOW:
            inCount = 0;
            outCount = 0;
            showText = false;

            if (setTemp != temp.get_lowValue())
                temp.set_lowValue(setTemp);

            mDisplayState = SHOW_TEMP;
            mBusy = false;
            break;
        default:
            SevenSegment.setState(SEGMENT_SET);
            _delay_ms(1000);

            mDisplayState = SET_HIGH;
            break;
    }
}


uint8_t segmentCount = 0;
bool tempState = false;
void cDisplayController::run()
{
	static uint8_t inCount = 0;
	static uint8_t outCount = 0;
	static bool showText = false;


	switch (mDisplayState)
	{
	case SHOW_TEMP:
	{
		static uint8_t sampleCount = 0;
		static uint32_t samples = 0;

		if(sampleCount++ < 16)
			samples += temp.adc_getTemp();
		else
		{
			sampleCount = 0;
			SevenSegment.setState(SEGMENT_NUMBER);
			samples = (samples >> 4);
			SevenSegment.setNumber(samples);
			samples = 0;
		}


	}break;
	case SHOW_HIGH:
		if (showTextNumber(SEGMENT_HI, temp.get_highValue(), SHOW_COUNT))
		    mDisplayState = SHOW_LOW;
		break;
	case SHOW_LOW:
	    if (showTextNumber(SEGMENT_LO, temp.get_lowValue(), SHOW_COUNT))
	        mDisplayState = SHOW_TEMP;
	    break;
	case SET_HIGH:
	    if (showTextNumber(SEGMENT_HI, setTemp, SHOW_COUNT_TIMEOUT))
	        mDisplayState = SHOW_TEMP;
	    break;
	case SET_LOW:
	    if (showTextNumber(SEGMENT_LO, setTemp, SHOW_COUNT_TIMEOUT))
	        mDisplayState = SHOW_TEMP;
	}

}

cDisplayController::~cDisplayController()
{

}

void light(uint8_t argc, char **argv)
{
	printf("temp: %d\n",temp.adc_getTemp());
}
extern const dbg_entry testEntry =
{ light, "t" };

void lowVal(uint8_t argc, char **argv)
{
	if (argc == 1)
	{
		printf("lo: %d\n", temp.get_lowValue());
		return;
	}

	uint8_t tempVal = atoi(argv[1]);
	temp.set_lowValue(tempVal);
}
extern const dbg_entry loEntry =
{ lowVal, "lo" };

void hiVal(uint8_t argc, char **argv)
{
	if (argc == 1)
	{
		printf("hi: %d\n", temp.get_highValue());
		return;
	}

	uint8_t tempVal = atoi(argv[1]);
	temp.set_highValue(tempVal);
}
extern const dbg_entry hiEntry =
{ hiVal, "hi" };
