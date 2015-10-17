// Do not remove the include below
#include "NeoWirelessLib.h"

void dumpClientConfiguration(client_configuration_t *config)
{
	printf("Client Configuration:\n");
	printf("Version: 0x%02x\n", config->version);
	printf("Node ID: 0x%02x\n", config->nodeId);
	printf("CRC: 0x%02x\n", config->crc);
}

uint8_t readClientConfiguration(client_configuration_t *config)
{
	uint8_t flag = false;
	uint8_t crc = 0;
	int address = CONFIG_START_ADDRESS;

	config->version = EEPROM.read(address++);
	config->nodeId = EEPROM.read(address++);;
	config->crc = EEPROM.read(address++);;

	crc = CRC8((const uint8_t *)config, 2);

	flag = (crc == config->crc);

	return flag;

}

uint8_t writeClientConfiguration(client_configuration_t *config)
{
	uint8_t flag = false;
	uint16_t address = CONFIG_START_ADDRESS;

	config->crc = CRC8((const uint8_t *)config, 2);
	EEPROM.write(address++, config->version);
	EEPROM.write(address++, config->nodeId);
	EEPROM.write(address, config->crc);

	flag = ( EEPROM.read(address) == config->crc );

	return flag;

}


//CRC-8 - based on the CRC8 formulas by Dallas/Maxim
//code released under the therms of the GNU GPL 3.0 license
uint8_t CRC8(const uint8_t *data, uint8_t len)
{
	uint8_t crc = 0x00;
	while (len--)
	{
		uint8_t extract = *data++;
		for (uint8_t tempI = 8; tempI; tempI--)
		{
			uint8_t sum = (crc ^ extract) & 0x01;
			crc >>= 1;
			if (sum)
			{
				crc ^= 0x8C;
			}
			extract >>= 1;
		}
	}
	return crc;
}

void clearAll()
{
	cmdFill = 0;
	cmdFillPattern = 0;
	cmdPattern = 0;
	cmdWipe = 0;
	cmdBounce = 0;
	cmdMiddle = 0;
	cmdRandomFlash = 0;
	cmdRainbow = 0;
	cmdRainbowFade = 0;
	cmdConfetti = 0;
	cmdCylon = 0;
	cmdBPM = 0;
	cmdJuggle = 0;

}

void dumpBuffer(uint8_t *buf, uint8_t len)
{
	printf("Command=");
	for (uint8_t i = 0; i < len; i++)
	{
		printf("%.2X ", buf[i]);
	}
	printf("\n\r");
}
//
//void dumpCommandBounce(uint8_t *buf)
//{
//	bounce_t *cmd;
//	cmd = (bounce_t *)buf;
//
//	printf("Command: %d\r\n", cmd->command);
//	printf("Pattern: %d\r\n", cmd->pattern);
//	printf("Direction: %d\r\n", cmd->direction);
//	printf("Color1: 0x%08lx\r\n", cmd->onColor);
//	printf("Color2: 0x%08lx\r\n", cmd->offColor);
//	printf("onTime: 0x%08lx\r\n", cmd->onTime);
//	printf("offTime: 0x%08lx\r\n", cmd->offTime);
//	printf("bounceTime: 0x%08lx\r\n", cmd->bounceTime);
//	printf("clearAfter: 0x%02x\r\n", cmd->clearAfter);
//	printf("clearEnd: 0x%02x\r\n", cmd->clearEnd);
//
//}
//void dumpCommandRunway(uint8_t *buf)
//{
//	runway_t *cmd;
//	cmd = (runway_t *)buf;
//
//	printf("Command: %d\r\n", cmd->command);
//	printf("Total: %d\r\n", cmd->pattern);
//	printf("Direction: %d\r\n", cmd->direction);
//	printf("Color1: 0x%08lx\r\n", cmd->onColor);
//	printf("Color2: 0x%08lx\r\n", cmd->offColor);
//	printf("onTime: 0x%08lx\r\n", cmd->onTime);
//	printf("offTime: 0x%08lx\r\n", cmd->offTime);
//	printf("clearAfter: 0x%02x\r\n", cmd->clearAfter);
//	printf("clearEnd: 0x%02x\r\n", cmd->clearEnd);
//
//}
//
//void dumpCommandSetAll(uint8_t *buf)
//{
//	set_all_t *cmd;
//	cmd = (set_all_t *)buf;
//
//	printf("Command: %d\r\n", cmd->command);
//	printf("Color: 0x%08lx\r\n", cmd->color);
//
//}
