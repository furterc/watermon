#ifndef PORT_H_
#define PORT_H_
#include <avr/io.h>

#define PORT_PB(_pin)		(       _pin)
#define PORT_PC(_pin)		(0x10 | _pin)
#define PORT_PD(_pin)		(0x20 | _pin)

class cPort
{
protected:
	uint8_t mPinDefinition;

	uint8_t PORTindex();
	volatile uint8_t *DDRn_reg();
	volatile uint8_t *PORTn_reg();
	volatile uint8_t *PINn_reg();

	void setDDRn();
	void clearDDRn();
	void setPORTn();
	void clearPORTn();


public:
	cPort(uint8_t pin_definition);
	virtual ~cPort();

	bool get();
};

#endif /* PORT_H_ */
