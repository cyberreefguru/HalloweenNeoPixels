// Do not remove the include below
#include "InterBoardComm.h"

/**
 * Constructor
 */
InterBoardComm::InterBoardComm()
{
}

/**
 * Initializes the library
 */
void InterBoardComm::initialize()
{
	commandReceived = false;
	Wire.begin();
}


/**
 * Initializes the library
 */
void InterBoardComm::initialize(uint8_t address)
{
	Wire.begin(address);
}

void InterBoardComm::onReceive(void (*function)(int) )
{
	Wire.onReceive(function);
}

volatile uint8_t* InterBoardComm::getRxBuffer()
{
	return rxBuffer;
}

volatile uint8_t InterBoardComm::getCommand()
{
	return command;
}

volatile uint8_t InterBoardComm::getCommandReceived()
{
	return commandReceived;
}

void InterBoardComm::setCommandReceived(volatile uint8_t commandReceived)
{
	this->commandReceived = commandReceived;
}


/**
 * Sends the command to the specified board
 */
void InterBoardComm::sendCommand(uint8_t address, uint8_t command, uint16_t len, uint8_t *buffer)
{
	Wire.beginTransmission(address);
	Wire.write(command);	// send command

	switch(command)
	{
	case COMMAND_START:
		break;
	case COMMAND_STOP:
		break;
	case COMMAND_WRITE:
		break;
	case COMMAND_BLANK:
		break;
	case COMMAND_CLEAR:
		break;
	case COMMAND_SET_BUFFER:
		// send buffer data
		break;
	case COMMAND_SET_PIXEL_NUMBER:
		// send pixel address 2 bytes
		Wire.write(buffer[0]);
		Wire.write(buffer[1]);
		// send pixel value 1 bytes
		Wire.write(buffer[2]);
		break;
	case COMMAND_SET_PIXEL_ADDRESS:
		// send pixel row 1 byte
		Wire.write(buffer[0]);
		// send pixel col 1 byte
		Wire.write(buffer[1]);
		// send pixel value 1 byte
		Wire.write(buffer[2]);
		break;
	case COMMAND_SET_ROW:
		// send pixel row 1 byte
		Wire.write(buffer[0]);
		// send pixel value 1 bytes
		Wire.write(buffer[1]);
		break;
	case COMMAND_SET_COL:
		// send pixel col 1 byte
		Wire.write(buffer[0]);
		// send pixel value 1 bytes
		Wire.write(buffer[1]);
		break;
	case COMMAND_SET_SEQUENCE:
		// send seq # 1 byte
		Wire.write(buffer[0]);
		break;
	case COMMAND_START_SEQUENCE:
		// send seq # 1 byte
		Wire.write(buffer[0]);
		break;
	default:
		break;
	}
	Wire.endTransmission();	// stop transmitting

//	waitForResponse(address, RESPONSE_ACK, 0, buffer);
}

void InterBoardComm::sendAck(uint8_t address)
{
	sendResponse(address, RESPONSE_ACK, 0, 0);
}


/**
 * Sends the command to the specified board
 */
void InterBoardComm::sendResponse(uint8_t address, uint8_t command, uint16_t len, uint8_t *buffer)
{
	Wire.beginTransmission(address);
	Wire.write(command);	// send command

	switch(command)
	{
	case RESPONSE_ACK:
		break;
	case RESPONSE_RESET_COMPLETE:
		// send one byte indicating which board reset
		Wire.write(buffer[0]);
		break;
	case RESPONSE_COMMAND_COMPLETE:
		// send the command that was completed
		Wire.write(buffer[0]);
		break;
	case RESPONSE_COMMAND_FAILURE:
		// send one byte with failure code
		Wire.write(buffer[0]);
		break;
	default:
		break;
	}
	Wire.endTransmission();	// stop transmitting
}



/**
 * Waits until we receive notification from other board
 */
void InterBoardComm::waitForResponse(uint8_t address, uint8_t command, uint16_t len, uint8_t *buffer)
{
	uint8_t c;
	uint16_t i=0;

	Wire.requestFrom((int)address, (int)(len+1));
	while(Wire.available() )
	{
		buffer[i++]=Wire.read();
	}

	if( buffer[0] != command )
	{
		error(0x03);
	}

//	switch(command)
//	{
//	case RESPONSE_ACK:
//		break;
//	case RESPONSE_RESET_COMPLETE:
//		// receive one byte indicating which board reset
//		while(!Wire.available());
//		buffer[0] = Wire.read();
//		break;
//	case RESPONSE_COMMAND_COMPLETE:
//		break;
//	case RESPONSE_COMMAND_FAILURE:
//		// receive one byte with failure code
//		while(!Wire.available());
//		buffer[0] = Wire.read();
//		break;
//	default:
//		break;
//	}
}


void InterBoardComm::receiveEvent(int len)
{
	while(Wire.available())
	{
		command = Wire.read();
		// continue to read command data
		switch(command)
		{
		case COMMAND_START:
			break;
		case COMMAND_STOP:
			break;
		case COMMAND_WRITE:
			break;
		case COMMAND_BLANK:
			break;
		case COMMAND_CLEAR:
			break;
		case COMMAND_SET_BUFFER:
			// send buffer data
			break;
		case COMMAND_SET_PIXEL_NUMBER:
			// send pixel address 2 bytes
			rxBuffer[0] = Wire.read();
			rxBuffer[1] = Wire.read();
			// send pixel value 1 bytes
			rxBuffer[2] = Wire.read();
			break;
		case COMMAND_SET_PIXEL_ADDRESS:
			// send pixel row 1 byte
			rxBuffer[0] = Wire.read();
			// send pixel col 1 byte
			rxBuffer[1] = Wire.read();
			// send pixel value 1 byte
			rxBuffer[2] = Wire.read();
			break;
		case COMMAND_SET_ROW:
			// send pixel row 1 byte
			rxBuffer[0] = Wire.read();
			// send pixel value 1 bytes
			rxBuffer[1] = Wire.read();
			break;
		case COMMAND_SET_COL:
			// send pixel col 1 byte
			rxBuffer[0] = Wire.read();
			// send pixel value 1 bytes
			rxBuffer[1] = Wire.read();
			break;
		case COMMAND_SET_SEQUENCE:
			// send seq # 1 byte
			rxBuffer[0] = Wire.read();
			break;
		case COMMAND_START_SEQUENCE:
			// send seq # 1 byte
			rxBuffer[0] = Wire.read();
			break;
		case RESPONSE_ACK:
			break;
		case RESPONSE_RESET_COMPLETE:
			// send one byte indicating which board reset
			rxBuffer[0] = Wire.read();
			break;
		case RESPONSE_COMMAND_COMPLETE:
			// send the command that was completed
			rxBuffer[0] = Wire.read();
			break;
		case RESPONSE_COMMAND_FAILURE:
			// send one byte with failure code
			rxBuffer[0] = Wire.read();
			break;
		default:
			break;
		}
		commandReceived = true;

	}
}



uint8_t InterBoardComm::delayWithRead(uint32_t value)
{
	uint8_t response = false;
	for(uint32_t i = 0; i< value; i++)
	{
		delay(1);
		if( commandReceived == true )
		{
			response = true;
			commandReceived = false;
			break;
		}
	}
	return response;
}

/**
 * flashes an error pattern
 */
void InterBoardComm::error(uint8_t code)
{
	while(1)
	{
		digitalWrite(13, HIGH);
		delay(code*100);
		digitalWrite(13, LOW);
		delay(code*100);
	}
}

InterBoardComm comm = InterBoardComm();

