/*
 * nvm.cpp
 *
 *  Created on: 14 Apr 2018
 *      Author: christo
 */

#include "nvm.h"
#include "terminal.h"

#define EEPROM_START	0

cNVM::cNVM() {
	// TODO Auto-generated constructor stub

}

cNVM::~cNVM() {
	// TODO Auto-generated destructor stub
}

void cNVM::write_bytes(uint8_t *addr, uint8_t *data, uint8_t len)
{
	for(uint8_t idx = 0; idx < len; idx++)
	{
		eeprom_write_byte(addr, data[idx]);
		eeprom_busy_wait();
		*addr++;
	}
}

void cNVM::read_bytes(uint8_t *addr, uint8_t *data, uint8_t len)
{
	for(uint8_t idx = 0; idx < len; idx++)
	{
		data[idx] = eeprom_read_byte(addr);
		eeprom_busy_wait();
		*addr++;
	}
}

void cNVM::setAddr(sNvm_wifi_address_t *nvm_addr)
{
	uint8_t addr = EEPROM_START;
	write_bytes(&addr, (uint8_t *)nvm_addr, sizeof(sNvm_wifi_address_t));
}

void cNVM::getAddr(sNvm_wifi_address_t *nvm_addr)
{
	uint8_t addr = EEPROM_START;
	read_bytes(&addr, (uint8_t *)nvm_addr, sizeof(sNvm_wifi_address_t));
}
