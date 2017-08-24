
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>


#include "terminal.h"
#include "commands.h"
#include "output.h"
#include "temp.h"
#include "buzzer.h"
#include "button.h"
#include "display_controller.h"



cBuzzer Buzzer = cBuzzer();
cButton Button = cButton();

void watchdogReset()
{
	__asm__ __volatile__ ( "wdr\n" );
}



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

//	SevenSegment.setNumber(number);
}
extern const dbg_entry numberEntry =
{ segmentNumber, "n" };

void btnCallback(bool state, uint8_t count)
{
	printf("button: %d - %d\n", state, count);
	static uint8_t btnCount = 0;
	if(!state)
		btnCount = count;

	if (state)
	{
		if(btnCount > 10)
			printf("Long press\n");
		else if(btnCount)
		{
			printf("Short press\n");
			DisplayController.show_info();
		}

	}
}

int main(void)
{

	sei();
	printp("main()\n");

	Button.setCB(&btnCallback);

	DisplayController.show_temp();

	uint16_t segmentCount = 0;
	bool tempState = false;
	while(1)
	{
		if (++segmentCount > 10)
		{
			DisplayController.run();
//			uint8_t temperature = (uint8_t)temp.adc_getTemp();
//
////			SevenSegment.setNumber(temperature);
//			segmentCount = 0;
//			if (!tempState)
//			{
//				SevenSegment.setState(SevenSegment.HI);
//				tempState = true;
//			}
//			else
//			{
//				SevenSegment.setState(SevenSegment.LO);
//				tempState = false;
//			}

		}


		Terminal.run();
		Button.run();

		_delay_ms(100);

	}
}
