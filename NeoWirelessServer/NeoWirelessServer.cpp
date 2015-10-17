/**
 * Copyright (c) 2015 Thomas M. Sasala
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT license.
 *
 */

#include "NeoWirelessServer.h"

void check_radio(void);

// Hardware configuration
RF24 radio(7, 8); // ce=7, cs=8

// Set address for channels
byte addresses[][6] = {"1node", "2node", "3node", "4node", "5node", "6node" };

#define NUM_NODES	2

static uint32_t message_count = 0;
static uint32_t message_good = 0;
static uint32_t message_fail = 0;
static uint8_t commandBuffer[MAX_COMMAND_SIZE];

uint8_t sendCommand();
uint8_t sendCommand(uint8_t channel);

void test();

/**
 *
 */
void setup()
{

    Serial.begin(115200);
    printf_begin();
    printf("\n\rNeoPixelWirelessServer\n\r");

    // Setup and configure radio
    radio.begin();
    radio.enableAckPayload(); // enable payload ack
    radio.enableDynamicPayloads(); // Ack payloads are dynamic payloads

//    radio.openWritingPipe(addresses[0]);
//    radio.openReadingPipe(1, addresses[1]);
    radio.printDetails();
    delay(50);
//    attachInterrupt(0, check_radio, LOW); // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

    for (int i = 0; i < MAX_COMMAND_SIZE; i++)
    {
        commandBuffer[i] = 0xff;
    }

    radio.powerUp();                        //Power up the radio

}

/**
 *
 */
void loop()
{

    test();

}

/**
 * Tests all commands
 */void test()
{
     // Test fill
     printf("Fill-White\n\r");
     cmdFill = (fill_t *)commandBuffer;
     cmdFill->command = CMD_FILL;
     cmdFill->color = WHITE;
     sendCommand();
     delay(4000);
     printf("Fill-Red\n\r");
     cmdFill->color = RED;
     sendCommand();
     delay(4000);
     printf("Fill-Black\n\r");
     cmdFill->color = BLACK;
     sendCommand();
     delay(4000);

     // Test Fill Pattern
     printf("Fill Pattern\n\r");
     cmdFillPattern = (fill_pattern_t *) commandBuffer;
     cmdFillPattern->command = CMD_FILL_PATTERN;
     cmdFillPattern->pattern = 0x03;
	 cmdFillPattern->onColor = CYAN;
	 cmdFillPattern->offColor = CRGB::Brown;
	 sendCommand();
	 delay(4000);

    // Test Pattern
    printf("Pattern\n\r");
    cmdPattern = (pattern_t *) commandBuffer;
    cmdPattern->command = CMD_PATTERN;
    cmdPattern->repeat = 0;
    cmdPattern->pattern = 0x33;
    cmdPattern->direction = LEFT;
    cmdPattern->onColor = RED;
    cmdPattern->offColor = WHITE;
    cmdPattern->onTime = 250;
    cmdPattern->offTime = 250;
    sendCommand();
    delay(4000);

    // Test Runway
    printf("Wipe\n\r");
    cmdWipe = (wipe_t *) commandBuffer;
    cmdWipe->command = CMD_WIPE;
    cmdWipe->pattern = 2;
    cmdWipe->direction = LEFT;
    cmdWipe->onColor = RED;
    cmdWipe->offColor = BLUE;
    cmdWipe->onTime = 100;
    cmdWipe->offTime = 0;
    cmdWipe->clearAfter = true;
    cmdWipe->clearEnd = true;
    sendCommand();
    delay(6000);

    cmdWipe->onTime = 50;
    sendCommand();
    delay(4000);

    cmdWipe->onTime = 25;
    sendCommand();
    delay(4000);

    // Test bounce
    printf("Bounce - Clear After\n\r");
    cmdBounce = (bounce_t *) commandBuffer;
    cmdBounce->command = CMD_BOUNCE;
    cmdBounce->repeat = 0;
    cmdBounce->pattern = 0x01;
    cmdBounce->direction = LEFT;
    cmdBounce->onColor = RED;
    cmdBounce->offColor = BLUE;
    cmdBounce->onTime = 25;
    cmdBounce->offTime = 0;
    cmdBounce->bounceTime = 0;
    cmdBounce->clearAfter = true;
    cmdBounce->clearEnd = true;
    sendCommand();
    delay(6000);

    // Test bounce
    printf("Bounce - No Clear After\n\r");
    cmdBounce->onColor = GREEN;
    cmdBounce->offColor = WHITE;
    cmdBounce->clearAfter = false;
    cmdBounce->clearEnd = false;
    sendCommand();
    delay(6000);

    // Clear after test
    cmdFill = (fill_t *)commandBuffer;
    cmdFill->command = CMD_FILL;
    cmdFill->color = BLACK;
    sendCommand();

    // Test Middle
    printf("Middle\n\r");
    cmdMiddle = (middle_t *) commandBuffer;
    cmdMiddle->command = CMD_MIDDLE;
    cmdMiddle->repeat = 0;
    cmdMiddle->direction = LEFT;
    cmdMiddle->onColor = RED;
    cmdMiddle->offColor = BLUE;
    cmdMiddle->onTime = 100;
    cmdMiddle->offTime = 0;
    cmdMiddle->clearAfter = true;
    cmdMiddle->clearEnd = true;
    sendCommand();
    delay(2000);
    cmdMiddle->direction = RIGHT;
    sendCommand();
    delay(2000);

     // Test random flash
     printf("Random Flash - Red/Black\n\r");
     cmdRandomFlash = (random_flash_t *) commandBuffer;
     cmdRandomFlash->command = CMD_RANDOM_FLASH;
     cmdRandomFlash->onColor = RED;
     cmdRandomFlash->offColor = BLACK;
     cmdRandomFlash->onTime = 25;
     cmdRandomFlash->offTime = 0;
     sendCommand();
     delay(5000);

     // Test random flash
     printf("Random Flash - White/Red\n\r");
     cmdRandomFlash = (random_flash_t *) commandBuffer;
     cmdRandomFlash->command = CMD_RANDOM_FLASH;
     cmdRandomFlash->onColor = WHITE;
     cmdRandomFlash->offColor = RED;
     cmdRandomFlash->onTime = 25;
     cmdRandomFlash->offTime = 0;
     sendCommand();
     delay(5000);

    // Test rainbow
    printf("Rainbow\n\r");
    cmdRainbow = (rainbow_t *) commandBuffer;
    cmdRainbow->command = CMD_RAINBOW;
    cmdRainbow->glitterProbability = 0;
    sendCommand();
    delay(4000);

    // Test rainbow
    printf("Rainbow with glitter\n\r");
    cmdRainbow = (rainbow_t *) commandBuffer;
    cmdRainbow->command = CMD_RAINBOW;
    cmdRainbow->glitterProbability = 80;
    cmdRainbow->glitterColor = WHITE;
    sendCommand();
    delay(4000);

    // Test rainbow
    printf("Rainbow with blue glitter\n\r");
    cmdRainbow = (rainbow_t *) commandBuffer;
    cmdRainbow->command = CMD_RAINBOW;
    cmdRainbow->glitterProbability = 80;
    cmdRainbow->glitterColor = BLUE;
    sendCommand();
    delay(4000);

    // Test rainbow fade
    printf("Rainbow Fade\n\r");
    cmdRainbowFade = (rainbow_fade_t *) commandBuffer;
    cmdRainbowFade->command = CMD_RAINBOW_FADE;
    sendCommand();
    delay(4000);

    // Test confetti
    printf("Confetti - 10\n\r");
    cmdConfetti = (confetti_t *) commandBuffer;
    cmdConfetti->command = CMD_CONFETTI;
    cmdConfetti->color = RED;
    cmdConfetti->numOn = 10;
    sendCommand();
    delay(4000);

    // Test confetti
    printf("Confetti - 5\n\r");
    cmdConfetti = (confetti_t *) commandBuffer;
    cmdConfetti->command = CMD_CONFETTI;
    cmdConfetti->color = GREEN;
    cmdConfetti->numOn = 5;
    sendCommand();
    delay(4000);

    // Test cylon
    printf("Cylon - Red\n\r");
    cmdCylon = (cylon_t *) commandBuffer;
    cmdCylon->command = CMD_CYLON;
    cmdCylon->repeat = 0;
    cmdCylon->color = RED;
    sendCommand();
    delay(4000);

    // Test BPM
    printf("BPM\n\r");
    cmdBPM = (bpm_t *) commandBuffer;
    cmdBPM->command = CMD_BPM;
    sendCommand();
    delay(4000);

    // Test Juggle
    printf("Juggle\n\r");
    cmdJuggle = (juggle_t *) commandBuffer;
    cmdBPM->command = CMD_JUGGLE;
    sendCommand();
    delay(4000);

}

/**
 * Send a command
 */
uint8_t sendCommand()
{
    uint8_t flag = false;

    for(uint8_t i=0; i < NUM_NODES; i++ )
    {
    	flag = sendCommand(i);
    }
//    flag = radio.writeFast(commandBuffer, MAX_COMMAND_SIZE);
//    if (!flag)
//    {
//        printf("Error sending Command: %d\n\r", commandBuffer[0]);
//        message_fail++;
//    }


    return flag;
}


uint8_t sendCommand(uint8_t channel)
{
	uint8_t flag = false;

    radio.openWritingPipe(addresses[channel]);
    flag = radio.write( commandBuffer, MAX_COMMAND_SIZE );
    if( flag == true )
    {
    	if( radio.available() )
    	{
            radio.read(&message_count, sizeof(message_count));
            printf("Ack Command 0x%02x from Node %d: %lu\n\r", commandBuffer[0], channel, message_count);
    	}
    	else
    	{
    		flag = false;
			printf("Error getting ACK Command 0x%02x from Node %d.\n\r", commandBuffer[0], channel);
    	}
    }
    else
    {
			printf("Error sending Command 0x%02x to Node %d.\n\r", commandBuffer[0], channel);
    }
	return flag;
}


/**
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
        //		printf("Send:OK\n\r");
    }

    if (fail)
    {
        // Have we failed to transmit?
        message_fail++;
        //		printf("Send:Failed\n\r");
    }

    if (rx || radio.available())
    {
        // Did we receive a message?
        radio.read(&message_count, sizeof(message_count));
        printf("Ack:%lu\n\r", message_count);
    }
}
