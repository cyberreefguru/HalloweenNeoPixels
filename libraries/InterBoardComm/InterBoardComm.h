// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef InterBoardComm_H_
#define InterBoardComm_H_
#include "Arduino.h"
#include <Wire.h>

#define MASTER_ADDRESS				0x10
#define TOWER_LIGHT_ADDRESS			0x20

#define COMMAND_START				0x01
#define COMMAND_STOP				0x02
#define COMMAND_WRITE				0x03
#define COMMAND_BLANK				0x04
#define COMMAND_CLEAR				0x05
#define COMMAND_RESET				0x06

#define COMMAND_SET_BUFFER			0x10
#define COMMAND_SET_PIXEL_NUMBER	0x11
#define COMMAND_SET_PIXEL_ADDRESS	0x12
#define COMMAND_SET_ROW				0x13
#define COMMAND_SET_COL				0x14

#define COMMAND_SET_SEQUENCE		0x20
#define COMMAND_START_SEQUENCE		0x21


#define RESPONSE_RESET_COMPLETE		0x50
#define RESPONSE_ACK				0x51
#define RESPONSE_COMMAND_COMPLETE	0x52
#define RESPONSE_COMMAND_FAILURE	0x53


class InterBoardComm
{
public:
	InterBoardComm();
	void initialize();
	void initialize(uint8_t address);
	void sendCommand(uint8_t address, uint8_t command, uint16_t len, uint8_t *buffer);
	void sendResponse(uint8_t address, uint8_t command, uint16_t len, uint8_t *buffer);
	void waitForResponse(uint8_t address, uint8_t command, uint16_t len, uint8_t *buffer);

	volatile uint8_t getCommandReceived();
	void setCommandReceived(volatile uint8_t commandReceived);
	void onReceive( void (*function)(int) );
	void receiveEvent(int len);
	volatile uint8_t getCommand();
	volatile uint8_t* getRxBuffer();

	uint8_t delayWithRead(uint32_t value);

	// Convenience functions
	void sendAck(uint8_t address);
	void sendReset(uint8_t address);
	void sendComplete(uint8_t address, uint8_t command);
	void sendError(uint8_t address, uint8_t errorCode);

private:
	void error(uint8_t code);
	volatile uint8_t command;
	volatile uint8_t commandReceived;
	volatile uint8_t rxBuffer[8];

};

//extern InterBoardComm comm;

#endif /* InterBoardComm_H_ */
