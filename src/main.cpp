
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>


#include "terminal.h"
#include "commands.h"
#include "output.h"
#include "temp.h"
#include "seven_segment.h"
#include "buzzer.h"


cTemp temp = cTemp();
cBuzzer Buzzer = cBuzzer();

void watchdogReset()
{
	__asm__ __volatile__ ( "wdr\n" );
}

void light(uint8_t argc, char **argv)
{
	temp.adc_getTemp();
}
extern const dbg_entry testEntry =
{ light, "t" };

void buzzer(uint8_t argc, char **argv)
{
	uint8_t duty = atoi(argv[1]);
	printp("duty: %d\n", duty);
//	pwmBuzzer.setDutyC(duty);
}
extern const dbg_entry buzzerEntry =
{ buzzer, "b" };

void segmentNumber(uint8_t argc, char **argv)
{
	if (argc != 2)
		return;

	uint16_t number = atoi(argv[1]);
	printp("show number: %d\n", number);

	SevenSegment.setNumber(number);
}
extern const dbg_entry numberEntry =
{ segmentNumber, "n" };

int main(void)
{

	sei();
	printp("main()\n");
//	timerPwm.init();

	uint16_t segmentCount = 0;
	bool tempState = false;
	while(1)
	{
		if (++segmentCount > 10)
		{
//			uint8_t temperature = (uint8_t)temp.adc_getTemp();

//			SevenSegment.setNumber(temperature);
			segmentCount = 0;
			if (!tempState)
			{
				SevenSegment.setState(SevenSegment.HI);
				tempState = true;
			}
			else
			{
				SevenSegment.setState(SevenSegment.LO);
				tempState = false;
			}

		}


		Terminal.run();
		_delay_ms(100);

	}
}
