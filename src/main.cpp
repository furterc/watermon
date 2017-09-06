
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

#include "terminal.h"
#include "output.h"
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

#ifdef _DEBUG
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
	printp("show number: %d\n", number);

//	SevenSegment.setNumber(number);
}
extern const dbg_entry numberEntry =
{ segmentNumber, "n" };

#endif

void btnCallback(bool state, uint8_t count)
{
    if(count > BUTTON_LONG_PRESS)
        return;

    static bool longPress = false;

    if( !state )
    {
        if( count == BUTTON_LONG_PRESS )
        {
            dbg_printp("Long press\n");
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

        dbg_printp("Short press\n");
        TempController.btnShortPress();
    }
}

int main(void)
{
	sei();
	dbg_printp("main()\n");
	Buzzer.init();

	Button.setCB(&btnCallback);

	uint16_t segmentCount = 0;
	while(1)
	{
		if (++segmentCount > 10)
		{
		    TempController.run();
		}
		Temp.run();
		Button.run();
#ifdef _DEBUG
		Terminal.run();
#endif

//		printf("w: %d\n", TempController.checkWater());
		_delay_ms(100);
	}
}

ISR(TIMER1_OVF_vect)
{
    SevenSegment.run();
}
