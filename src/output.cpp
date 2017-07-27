#include "output.h"

cOutput::cOutput(uint8_t pin) : cPort(pin)
{
	setDDRn();

	reset();
}

void cOutput::set()
{
	setPORTn();
}

void cOutput::reset()
{
	clearPORTn();
}

cOutput::~cOutput()
{
	clearDDRn();
}

