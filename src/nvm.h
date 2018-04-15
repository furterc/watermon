/*
 * nvm.h
 *
 *  Created on: 14 Apr 2018
 *      Author: christo
 */

#ifndef SRC_NVM_H_
#define SRC_NVM_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdint.h>

typedef struct{
	uint8_t tx_address[5];
	uint8_t rx_address[5];
	uint8_t slaveHost;
}sNvm_wifi_address_t;

class cNVM {


	void write_bytes(uint8_t *addr, uint8_t *data, uint8_t len);
	void read_bytes(uint8_t *addr, uint8_t *data, uint8_t len);

public:
	cNVM();
	virtual ~cNVM();

	void setAddr(sNvm_wifi_address_t *nvm_addr);
	void getAddr(sNvm_wifi_address_t *nvm_addr);
};

#endif /* SRC_NVM_H_ */
