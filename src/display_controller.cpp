/*
 * display_controller.cpp
 *
 *  Created on: 28 Jul 2017
 *      Author: christo
 */

#include "display_controller.h"

#include <stdlib.h>
#include <avr/delay.h>

#include "seven_segment.h"
#include "temp.h"
#include "terminal.h"
#include "commands.h"

#define SHOW_THRESHHOLD 3
#define SHOW_COUNT		1

cTemp temp = cTemp();

cDisplayController DisplayController;

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
}

bool cDisplayController::set_mode_busy()
{
    if (!mBusy)
        return false;



    return true;
}

void cDisplayController::enter_set_mode()
{
    mBusy = true;
    switch (mDisplayState) {
        case SET_HIGH:
            mDisplayState = SET_LOW;
            break;
        case SET_LOW:
            mDisplayState = SHOW_TEMP;
            mBusy = false;
            break;
        default:
            SevenSegment.setState(SevenSegment.SET);
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
			SevenSegment.setState(SevenSegment.NUMBER);
			samples = (samples >> 4);
			SevenSegment.setNumber(samples);
			samples = 0;
		}


	}break;
	case SHOW_HIGH:
		if(inCount++ > SHOW_THRESHHOLD)
		{
			inCount = 0;

			if (outCount >= SHOW_COUNT)
			{
				mDisplayState = SHOW_LOW;
				outCount = 0;
				showText = false;
				return;
			}


			if(!showText)
			{
				SevenSegment.setState(SevenSegment.HI);
				showText = true;
			}
			else
			{
				SevenSegment.setNumber(temp.get_highValue());
				showText = false;
				outCount++;
			}
		}
		break;
	case SHOW_LOW:
		if(inCount++ > SHOW_THRESHHOLD)
		{
			inCount = 0;

			if (outCount >= SHOW_COUNT)
			{
				mDisplayState = SHOW_TEMP;
				outCount = 0;
				showText = false;
				return;
			}

			if(!showText)
			{
				SevenSegment.setState(SevenSegment.LO);
				showText = true;
			}
			else
			{
				SevenSegment.setNumber(temp.get_lowValue());
				showText = false;
				outCount++;
			}
		}
		break;

	case SET_HIGH:
	    SevenSegment.setState(SevenSegment.HI);
		break;
	case SET_LOW:
	    SevenSegment.setState(SevenSegment.LO);
		break;
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
