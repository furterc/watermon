
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include "nvm.h"

#include "terminal.h"

void watchdogReset()
{
	__asm__ __volatile__ ( "wdr\n" );
}

#include "commands.h"
#include "nrf24.h"
#include <string.h>

cNVM NVM = cNVM();

uint8_t checkTx()
{
	uint8_t tmp = nrf24_lastMessageStatus();

	if(tmp == NRF24_TRANSMISSON_OK)
	{
		printf("tx ok\n");
		return 1;
	}
	else if(tmp == NRF24_MESSAGE_LOST)
	{
		printf("tx failed\n");
		return 0;
	}

	return 0;
}

void printAddr()
{
	sNvm_wifi_address_t nvm;
	NVM.getAddr(&nvm);

	printf("slaveHost: %02X\n", nvm.slaveHost);
	if(!nvm.slaveHost)
	{
		printp("Host:\n");
	}
	else if(nvm.slaveHost == 1)
	{
		printp("Slave:\n");
	}
	else
	{
		printp("Both:\n");
	}

	printp("txAddr:");
	for(uint8_t idx = 0; idx < 5; idx++)
		printp(" %02X", nvm.tx_address[idx]);
	printp("\nrxAddr:");
	for(uint8_t idx = 0; idx < 5; idx++)
		printp(" %02X", nvm.rx_address[idx]);
	printp("\n");
}

uint8_t sendArray()
{
	uint8_t data_array[4];

	data_array[0] = 0x00;
	data_array[1] = 0xAA;
	data_array[2] = 0x55;
	data_array[3] = 0xFF;
	nrf24_send(data_array);
	printp("sending 4 bytes\n");

	while(nrf24_isSending());

	return checkTx();
}

void send(uint8_t argc, char **argv)
{
	sendArray();
	nrf24_powerUpRx();
}
extern const dbg_entry sendEntry =
{ send, "s" };

void check(uint8_t argc, char **argv)
{
	checkTx();
	nrf24_powerUpRx();
}
extern const dbg_entry checkEntry =
{ check, "c" };

void slaveSelect(uint8_t argc, char **argv)
{
	if(argc != 2)
	{
		printp(" 0 -> host\n 1 -> slave\n 2 -> both\n");
		return;
	}
	sNvm_wifi_address_t addr;

	NVM.getAddr(&addr);

	uint8_t tmp = atoi(argv[1]);
	printf("tmp: %d\n", tmp);
	addr.slaveHost = tmp;
	printf("slaveHost: %02X\n", addr.slaveHost);

	NVM.setAddr(&addr);
//	printAddr();
}
extern const dbg_entry slaveHostEntry =
{ slaveSelect, "sh" };

void charToHex(char *characters, uint8_t *hexData, uint8_t len)
{
	char byte[2];
	uint8_t charIdx = 0;
	for(uint8_t idx = 0; idx < len; idx++)
	{
		memcpy(byte, &characters[charIdx], 2);
		charIdx += 2;
		hexData[idx] = (uint8_t)strtol(byte, NULL, 16);
	}
}

void Address(uint8_t argc, char **argv)
{
	if(argc == 1)
	{
		printAddr();
		return;
	}

	if (argc != 3)
	{
		printp("addr <tx(0)/rx(1)> <addr[5]>");
		return;
	}

	sNvm_wifi_address_t nvm;
	NVM.getAddr(&nvm);

	if(strlen(argv[2]) != 10)
	{
		printp("5 bytes for addr\n");
		return;
	}

	uint8_t addr[5];
	charToHex(argv[2], addr, 5);

	uint8_t txRx = atoi(argv[1]);
	if(txRx == 0)
	{
		memcpy(nvm.tx_address, addr, 5);
	} else if(txRx == 1)
	{
		memcpy(nvm.rx_address, addr, 5);
	} else
		return;

	NVM.setAddr(&nvm);

}
extern const dbg_entry numberEntry =
{ Address, "n" };


int main(void)
{
	sei();

	Terminal.init();

	sNvm_wifi_address_t nvm;
	NVM.getAddr(&nvm);

	nrf24_init();
	nrf24_config(2,4);
	nrf24_rx_address(nvm.rx_address);
	nrf24_tx_address(nvm.tx_address);
	printp("nrf_init()\n");
	nrf24_powerUpRx();
	printp("nrf_rx_on\n");
	printAddr();

	uint8_t slaveHost = nvm.slaveHost;
	uint8_t data[4];
	while(1)
	{
		_delay_ms(100);
		Terminal.run();

		if(!slaveHost)
		{
			static uint8_t idx = 0;
			idx++;
			if(idx > 20)
			{
				idx = 0;
				sendArray();
			}
		}

		if(slaveHost && nrf24_dataReady())
		{
			nrf24_getData(data);
			printp("rx:");
			for(uint8_t idx = 0; idx < 4; idx++)
				printp(" %02X", data[idx]);
			printp("\n");
		}

	}
}
