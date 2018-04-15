
#include "terminal.h"

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "mystdio.h"
#include "commands.h"

cTerminal::cTerminal()
{
	mHead = 0;
	mTail = 0;
	mystdio_init();
}

void cTerminal::init()
{


	UCSR0A = _BV(U2X0); //Double speed mode USART0
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	UBRR0L = (uint8_t)( (F_CPU + 115200 * 4L) / (115200 * 8L) - 1 );

	printf("Terminal started\n");
}

void  cTerminal::enableInput()
{
	UCSR0B |= _BV(RXCIE0);
}

void  cTerminal::disableInput()
{
	UCSR0B &= ~(_BV(RXCIE0));
}

void cTerminal::run()
{
	while(mHead != mTail)
	{
		if((mCommand[mTail] == '\n') || (mCommand[mTail] == '\r'))
		{
			disableInput();
			putchar('\n');
			handleCommand();
			printp("#>");
			enableInput();
		}
		else
			mTail++;
	}
}

uint8_t cTerminal::parseCommand(char *command, uint8_t argc, char **argv)
{
	uint8_t count = 0;
	argc -= 2;
	argv[count] = command;
	char *ptr = strchr(argv[count], ' ');

	while(ptr && (count++ < argc))
	{
		ptr[0] = 0;
		ptr++;

		argv[count] = ptr;

		ptr = strchr(argv[count], ' ');
	}

	count++;

	return count;
}

void cTerminal::handleCommand()
{
	mCommand[mTail] = 0;
	//printf("CMD: %s\n", mCommand);
	mHead = 0;
	mTail = 0;

	if(!strlen(mCommand))
		return;

	char* argv[5];
	uint8_t argc = 0;

	argc = parseCommand(mCommand, 5, argv);

	if(!argc)
		return;

	uint8_t idx = 0;
	const dbg_entry *currEntry = dbg_entries[idx++];
	while(currEntry)
	{
		//printp("%s - %s\n", currEntry->command, mCommand);
		if(!strcmp(currEntry->command, mCommand))
		{
			currEntry->func(argc, argv);
			return;
		}

		currEntry = dbg_entries[idx++];
	}

	printp("unknown command\n");
}

void cTerminal::handle(char ch)
{
	putchar(ch);
	mCommand[mHead] = ch;

	if(++mHead > 63)
	{
		mHead = 0;
		mTail = 0;
	}
}

void help(uint8_t argc, char **argv)
{
	printp("Try:\n", argc);
	uint8_t idx = 0;
	const dbg_entry *currEntry = dbg_entries[idx++];
	while(currEntry)
	{
		printp(" - %s\n", currEntry->command);

		currEntry = dbg_entries[idx++];
	}
}


cTerminal::~cTerminal()
{
}

ISR(USART_RX_vect)
{
	char ch = UDR0;
	Terminal.handle(ch);
}

const dbg_entry helpEntry = {help, "h"};

cTerminal Terminal;
