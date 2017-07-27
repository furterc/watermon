#include "port.h"


const volatile uint8_t* DDRn[] =
{
		&DDRB,
		&DDRC,
		&DDRD,
};

const volatile uint8_t* PORTn[] =
{
		&PORTB,
		&PORTC,
		&PORTD,
};

const volatile uint8_t* PINn[] =
{
		&PINB,
		&PINC,
		&PIND,
};

cPort::cPort(uint8_t pin_definition) : mPinDefinition(pin_definition)
{
}
uint8_t cPort::PORTindex()
{
	return ((mPinDefinition & 0xF0) >> 4);
}

volatile uint8_t* cPort::DDRn_reg()
{
	uint8_t portIdx = PORTindex();
	return (volatile uint8_t *)DDRn[portIdx];
}

volatile uint8_t* cPort::PORTn_reg()
{
	uint8_t portIdx = PORTindex();
	return (volatile uint8_t *)PORTn[portIdx];
}

volatile uint8_t* cPort::PINn_reg()
{
	uint8_t portIdx = PORTindex();
	return (volatile uint8_t *)PINn[portIdx];
}

void cPort::setDDRn()
{
	volatile uint8_t *port = DDRn_reg();
	*port |= 1 << (mPinDefinition & 0x0F);
}

void cPort::clearDDRn()
{
	volatile uint8_t *port = DDRn_reg();
	*port &= ~(1 << (mPinDefinition & 0x0F));
}

void cPort::setPORTn()
{
	volatile uint8_t *port = PORTn_reg();
	*port |= 1 << (mPinDefinition & 0x0F);
}

void cPort::clearPORTn()
{
	volatile uint8_t *port = PORTn_reg();
	*port &= ~(1 << (mPinDefinition & 0x0F));
}


bool cPort::get()
{
	 volatile uint8_t *port = PINn_reg();

	 if( *port & (1 << (mPinDefinition & 0x0F)))
		 return true;

	 return false;
}

cPort::~cPort()
{
}

