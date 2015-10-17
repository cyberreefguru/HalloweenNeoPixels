/**
 * Copyright (c) 2015 Thomas M. Sasala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT license.
 *
 */
#include "NeoWirelessClient.h"

void commandMode();
void check_radio(void);
void parseCommand();
void setNodeId(uint8_t id);

NeoPixelWrapper controller = NeoPixelWrapper();
volatile uint32_t endTime;

boolean isCommandAvailable();

// Hardware configuration
RF24 radio(7, 8); // ce=7, cs=8

// Set address for channels
byte addresses[][6] = { "1node" };

static uint32_t message_count = 0;
static uint32_t message_good = 0;
static uint32_t message_fail = 0;
static volatile uint8_t commandAvailable = false;
static volatile uint8_t commandBuffer[MAX_COMMAND_SIZE];

client_configuration_t clientConfig;

/**
 *
 */
void setup()
{

	Serial.begin(115200);
	printf_begin();
	Serial.println(F("\n\n** NeoPixelWirelessClient ** \n\n"));

	Serial.println(F("Reading client configuration...."));
	if( !readClientConfiguration( (client_configuration_t *)&clientConfig) )
	{
		Serial.println(F("** Error reading client configuration\n"));
		clientConfig.version = CLIENT_CONFIG_V10;
		clientConfig.nodeId = 0x01;
		if( !writeClientConfiguration((client_configuration_t *)&clientConfig) )
		{
			Serial.println(F("** Error writing client configuration\n"));
		}
		else
		{
			Serial.println(F("Successfully wrote client configuration\n"));
		}
	}
	else
	{
		Serial.println(F("Successfully Read Configuration:\n"));
	}
	dumpClientConfiguration((client_configuration_t *)&clientConfig);

	// Setup and configure radio
	radio.begin();
	radio.enableAckPayload(); // enable payload ack
	radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

	setNodeId(clientConfig.nodeId);
//	radio.openWritingPipe(addresses[1]);
//	radio.openReadingPipe(1, addresses[0]);
//	radio.startListening(); // we're the client, so start listening
	radio.writeAckPayload(1, &message_count, sizeof(message_count));
	++message_count;
	radio.printDetails(); // Dump the configuration of the rf unit for debugging
	delay(50);
	attachInterrupt(0, check_radio, LOW); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

	if (controller.initialize(50, 2) == false)
	{

	}
	else
	{
		controller.fill(CRGB::Black, true);
	}

}

/**
 *
 */
void loop()
{

	// Wait for command
	if (commandAvailable)
	{
		parseCommand();

	} // end commandAvailable

	if( Serial.available() )
	{
		commandMode();
	}

} // end loop

void setNodeId(uint8_t id)
{
	// Set the node ID
	addresses[0][0] = clientConfig.nodeId;
	// Stop listening
	radio.stopListening();
	// Open new pipe with new ID
	radio.openReadingPipe(1, addresses[0]);
	// Strart listening for commands
	radio.startListening();
}
/**
 * Handles serial commands and changing configuration
 */
void commandMode()
{
	uint8_t id = 0;

	// Read first character - discard since it just gets us into command mode
	char c = toupper(Serial.read());
	Serial.println(F("COMMAND MODE:"));
	Serial.println(F("C - Change Node ID"));
	Serial.println(F("D - Dump Configuration"));
	Serial.println(F("E - exit"));
	while(!Serial.available() ){}

	c = toupper(Serial.read());
	switch( c )
	{
	case 'C':
		Serial.print(F("** Change Node Id **\nCurrent "));
		dumpClientConfiguration(&clientConfig);

		Serial.print(F("\nEnter 1 digit node ID: "));
		while(!Serial.available() ){}
		id = Serial.read();
		Serial.println( (char)id ); // echo what the user typed
		Serial.println(F("\nProcessing...\n"));
		if( id >= '0' && id <= '9' )
		{
			// set node id
			clientConfig.nodeId = id;
			dumpClientConfiguration(&clientConfig);
			if( writeClientConfiguration(&clientConfig) )
			{
				Serial.print(F("Node ID accepted: "));
				Serial.println( clientConfig.nodeId, HEX);

				setNodeId(id);
//				radio.stopListening();
//				addresses[0][0] = clientConfig.nodeId;
//				radio.openReadingPipe(1, addresses[0]);
//				radio.startListening(); // we're the client, so start listening
				radio.printDetails(); // Dump the configuration of the rf unit for debugging

			}
			else
			{
				Serial.println(F("Error saving configuration information."));
			}
		}
		else
		{
			Serial.print(F("Illegal node value entered:"));
			Serial.println(id);
		}
		break;
	case 'D':
		dumpClientConfiguration(&clientConfig);
		radio.printDetails();
		break;
	case 'E':
		break;
	}
}

/**
 * Handles interrupts from the radio
 *
 *
 */
void check_radio(void)
{

	bool tx, fail, rx;

	// What happened?
	radio.whatHappened(tx, fail, rx);

	if (tx)
	{
		// Have we successfully transmitted?
		message_good++;
//		printf("Ack Payload:Sent\n\r");
	}

	if (fail)
	{
		// Have we failed to transmit?
		message_fail++;
//		printf("Ack Payload:Failed\n\r");
	}

	// Did we receive a message?
	if (rx || radio.available())
	{

		// get payload and store in buffer; set command flag
		while (radio.available())
		{
			radio.read((void *) commandBuffer, MAX_COMMAND_SIZE);
		}
		radio.writeAckPayload(1, &message_count, sizeof(message_count));
		++message_count;
		commandAvailable = true;
	}
}

void parseCommand()
{
	commandAvailable = false;
	printf("COMMAND RECEIVED: 0x%02x - ", commandBuffer[0]);
	switch (commandBuffer[0])
	{

	case CMD_FILL:
		printf("FILL\n\r");
		cmdFill = (fill_t *) commandBuffer;
		controller.fill(cmdFill->color, true);
		break;
	case CMD_FILL_PATTERN:
		printf("FILL_PATTERN\n\r");
		cmdFillPattern = (fill_pattern_t *) commandBuffer;
		controller.fillPattern(cmdFillPattern->pattern, cmdFillPattern->onColor, cmdFillPattern->offColor);
		break;
	case CMD_PATTERN:
		printf("PATTERN\n\r");
		cmdPattern = (pattern_t *) commandBuffer;
		controller.pattern(cmdPattern->repeat, cmdPattern->pattern, cmdPattern->direction, cmdPattern->onColor, cmdPattern->offColor, cmdPattern->onTime, cmdPattern->offTime);
		break;
	case CMD_WIPE:
		printf("WIPE\n\r");
		cmdWipe = (wipe_t *) commandBuffer;
		controller.wipe(cmdWipe->pattern, cmdWipe->direction, cmdWipe->onColor, cmdWipe->offColor, cmdWipe->onTime, cmdWipe->offTime, cmdWipe->clearAfter, cmdWipe->clearEnd);
		break;
	case CMD_BOUNCE:
		printf("BOUNCE\n\r");
		cmdBounce = (bounce_t *) commandBuffer;
		controller.bounce(cmdBounce->repeat, cmdBounce->pattern, cmdBounce->direction, cmdBounce->onColor, cmdBounce->offColor, cmdBounce->onTime, cmdBounce->offTime, cmdBounce->bounceTime, cmdBounce->clearAfter, cmdBounce->clearEnd);
		break;
	case CMD_MIDDLE:
		printf("MIDDLE\n\r");
		cmdMiddle = (middle_t *) commandBuffer;
		controller.middle(cmdMiddle->repeat, cmdMiddle->direction, cmdMiddle->onColor, cmdMiddle->offColor, cmdMiddle->onTime, cmdMiddle->offTime, cmdMiddle->clearAfter, cmdMiddle->clearEnd);
		break;
	case CMD_RANDOM_FLASH:
		printf("RANDOM_FLASH\n\r");
		cmdRandomFlash = (random_flash_t *) commandBuffer;
		controller.randomFlash(0, cmdRandomFlash->onTime, cmdRandomFlash->offTime, cmdRandomFlash->onColor, cmdRandomFlash->offColor);
		break;
	case CMD_RAINBOW:
		printf("RAINBOW\n\r");
		cmdRainbow = (rainbow_t *) commandBuffer;
		controller.rainbow(0, cmdRainbow->glitterProbability, cmdRainbow->glitterColor);
		break;
	case CMD_RAINBOW_FADE:
		printf("RAINBOW_FADE\n\r");
		cmdRainbowFade = (rainbow_fade_t *) commandBuffer;
		controller.rainbowFade(0);
		break;
	case CMD_CONFETTI:
		printf("CONFETTI\n\r");
		cmdConfetti = (confetti_t *) commandBuffer;
		controller.confetti(0, cmdConfetti->color, cmdConfetti->numOn);
		break;
	case CMD_CYLON:
		printf("CYLON\n\r");
		cmdCylon = (cylon_t *) commandBuffer;
		controller.cylon(cmdCylon->repeat, cmdCylon->color);
		break;
	case CMD_BPM:
		printf("BPM\n\r");
		cmdBPM = (bpm_t *) commandBuffer;
		controller.bpm(0);
		break;
	case CMD_JUGGLE:
		printf("JUGGLE\n\r");
		cmdJuggle = (juggle_t *) commandBuffer;
		controller.juggle(0);
		break;
	case CMD_SET_HUE_UPDATE_TIME:
		printf("SET_HUE_UPDATE_TIME\n\r");
		cmdSetHueUpdateTime = (set_hue_update_time_t *) commandBuffer;
		controller.setHueUpdateTime(cmdSetHueUpdateTime->updateTime);
		break;
	case CMD_SET_FPS:
		printf("SET_FPS\n\r");
		cmdSetFPS = (set_fps_t *) commandBuffer;
		controller.setFramesPerSecond(cmdSetFPS->fps);
		break;
	case CMD_ERROR:
	default:
		printf("ERROR\n\r");
		break;
	}

}

/**
 * Returns flag if a new command is available
 *
 */
boolean isCommandAvailable()
{
	return commandAvailable;
}

