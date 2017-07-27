#ifndef OUTPUT_H_
#define OUTPUT_H_
#include "port.h"

class cOutput : public cPort
{

public:
	cOutput(uint8_t pin);
	virtual ~cOutput();

	void set();
	void reset();
};

#endif /* OUTPUT_H_ */
