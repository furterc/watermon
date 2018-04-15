#ifndef TERMINAL_H_
#define TERMINAL_H_


#include <stdio.h>
#include <avr/pgmspace.h>

#define printp(_x, ...) printf_P(PSTR(_x), ##__VA_ARGS__);

typedef void (*dbg_func)(uint8_t argc, char **argv);

struct dbg_entry
{
	dbg_func func;
	const char *command;
};

void help();


class cTerminal
{
	uint8_t mHead;
	uint8_t mTail;
	char mCommand[64];

	void enableInput();
	void disableInput();

	uint8_t parseCommand(char *command, uint8_t argc,  char **argv);
	void handleCommand();

public:
	cTerminal();
	virtual ~cTerminal();

	void init();
	void run();
	void handle(char ch);
};

extern const dbg_entry helpEntry;
extern const dbg_entry* dbg_entries[];
extern cTerminal Terminal;

#endif /* TERMINAL_H_ */
