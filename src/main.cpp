
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

#include "terminal.h"
#include "temp.h"
#include "buzzer.h"
#include "button.h"
#include "display_controller.h"
#include "temp_controller.h"

#define BUTTON_LONG_PRESS   10    //n*100mS

cBuzzer Buzzer = cBuzzer();
cButton Button = cButton();

cTemp Temp = cTemp();

cSevenSegment SevenSegment = cSevenSegment();
cDisplayController DisplayController = cDisplayController(&SevenSegment);

cTempController TempController = cTempController(&Temp, &DisplayController, &Buzzer);

void watchdogReset()
{
	__asm__ __volatile__ ( "wdr\n" );
}

#include "commands.h"

void buzzer(uint8_t argc, char **argv)
{
	uint8_t duty = atoi(argv[1]);
}
extern const dbg_entry buzzerEntry =
{ buzzer, "b" };


void segmentNumber(uint8_t argc, char **argv)
{
	if (argc != 2)
		return;

	uint16_t number = atoi(argv[1]);
	printf("show number: %d\n", number);

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
            TempController.btnLongPress();
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
        TempController.btnShortPress();
    }
}

int main(void)
{
	printf("main()\n");
	Buzzer.init();

	Button.setCB(&btnCallback);

	sei();

	uint16_t segmentCount = 0;

	while(1)
	{
		if (++segmentCount > 10)
		{
		    TempController.run();
		}
		Temp.run();
		Button.run();
		Terminal.run();

		_delay_ms(100);
	}
}

ISR(TIMER1_OVF_vect)
{
    SevenSegment.run();
}
