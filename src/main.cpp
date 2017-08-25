
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

#define BUTTON_LONG_PRESS   10    //n*100mS

cBuzzer Buzzer = cBuzzer();
cButton Button = cButton();
cDisplayController DisplayController = cDisplayController();



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
    if(count > BUTTON_LONG_PRESS)
        return;

    static bool longPress = false;

    if( !state )
    {
        if( count == BUTTON_LONG_PRESS )
        {
            printf("Long press\n");
            DisplayController.enter_set_mode();
            longPress = true;
        }
    }
    else
    {
        if (longPress)
        {
            longPress = false;
            return;
        }

        printf("Short press\n");

        if ( !DisplayController.set_mode_busy())
            DisplayController.show_info();
    }
}

int main(void)
{
	sei();
	printp("main()\n");

	Button.setCB(&btnCallback);

	DisplayController.show_temp();

	uint16_t segmentCount = 0;
	while(1)
	{
		if (++segmentCount > 10)
		{
			DisplayController.run();
		}


		Terminal.run();
		Button.run();

		_delay_ms(100);

	}
}
