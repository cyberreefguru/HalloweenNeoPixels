/*
 * main.c
 *
 *  Created on: Dec 26, 2011
 *      Author: Thomas M. Sasala
 *
 *  Controller for Faller Top Spin
 *
 * Copyright (c) 2011 Thomas M. Sasala.  All right reserved.
 *
 * This work is licensed under the Creative Commons
 * Attribution-NonCommercial-ShareAlike 3.0 Unported License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View,
 * California, 94041, USA.
 *
 */

#include "TopSpin.h"

/**
 * Function stereotypes
 */
void step(uint8_t motorIndex, int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue);
void forward(uint8_t motorIndex, uint8_t speed);
void backward(uint8_t motorIndex, uint8_t speed);
void stop(uint8_t motorIndex);
void setSpeed(uint8_t motorIndex, uint8_t speed);
void initialize();
void testOutput();
void flash();
void sample();
void debounceInput( uint8_t pin, volatile uint8_t *currentValue, volatile uint8_t *currentCount, volatile uint8_t *change );
void findHome();
void fineTune();
void togglePin( uint8_t pin, uint8_t repeat, uint16_t duration );
void move( uint8_t direction, uint8_t motorIndex, uint8_t speed);
void rotateMainArm(uint8_t direction, uint8_t repeat, uint8_t speed);
void rotateChair(uint8_t direction, uint16_t duration, uint8_t speed);
void moveTimed(uint8_t direction, uint8_t motorIndex, uint8_t speed, uint16_t duration);


volatile uint8_t led;
volatile uint8_t currentBaseValue;
volatile uint8_t currentSideValue;
volatile uint8_t baseCount;
volatile uint8_t sideCount;
volatile uint8_t baseChange;
volatile uint8_t sideChange;

#define DEBOUNCE_DELAY	5


/**
 * Main application
 *
 */
int main(void)
{
	// Initialize libraries
	init();

	// Initialize hardware
	initialize();

#ifdef __DEBUG
	// Notify of program start
	Serial.println("PROGRAM START");
#endif

#ifdef __DEBUG
	Serial.println("Starting Motor");
#endif

	rotateMainArm( FORWARD, 5, 255 );
	delay(2000);
	rotateMainArm( BACKWARD, 5, 255 );
	delay(2000);
	moveTimed( FORWARD, MAIN_ARM, 255, 1600 );
	delay(4000);
	rotateChair( FORWARD, 5000, 255);
	delay(4000);
	rotateChair( BACKWARD, 5200, 255);
	delay(3000);
	rotateChair( FORWARD, 6200, 100);
	delay(4000);
	rotateChair( BACKWARD, 5900, 75);
	delay(6000);
	rotateChair( FORWARD, 5500, 75);
	delay(4000);
	rotateMainArm( BACKWARD, 5, 255 );
	moveTimed( BACKWARD, MAIN_ARM, 255, 500 );
	delay(3000);
	rotateChair( BACKWARD, 6200, 60);
	delay(1000);
	rotateChair( FORWARD, 5200, 255);
	delay(4000);
	rotateMainArm( BACKWARD, 2, 255 );
	moveTimed( BACKWARD, MAIN_ARM, 255, 350 );
	delay(1000);
	rotateChair( FORWARD, 6000, 75);
	delay(500);
	rotateChair( FORWARD, 5200, 255);
	delay(4000);

	findHome();
	delay(500);
	fineTune();
	while(1);

#ifdef __DEBUG
	// never get here, but...
	Serial.println("PROGRAM END");
#endif

	// Just in case
	while(1);

} // end main

/**
 * Rotates the main arm in the direction specified at the specified
 * speed for the number of times specified.
 *
 */
void rotateMainArm(uint8_t direction, uint8_t repeat, uint8_t speed)
{
	uint8_t i;

	// Start moving
	move(direction, MAIN_ARM, speed);
	delay(200); // wait to clear tower in case we're in home position

	// Wait until main arm passes base to start counting
	while( currentSideValue == 1 );
	while( currentSideValue == 0 );
	repeat = repeat-1;

	for(i=0; i<repeat; i++ )
	{
		// Wait until arm passes base
		while( currentSideValue == 1 );
		while( currentSideValue == 0 );
	}
	stop( MAIN_ARM );
}

/**
 * Rotates the chair in the direction specified at the specified
 * speed for the specified duration.
 *
 */
void rotateChair(uint8_t direction, uint16_t duration, uint8_t speed)
{
	// Start moving
	move(direction, CHAIR, speed);
	delay(duration);
	stop( CHAIR );
}


/**
 * Routine to put main arm in home position
 *
 */
void findHome()
{
	// No telling where we are, rotate forward
	forward(MAIN_ARM, 255);
	while( currentSideValue == 1 );
	while( currentSideValue == 0 );
	stop(MAIN_ARM);

	// Minor delay for dramatic effect
	delay(250);

	backward(MAIN_ARM, 100);
	while(currentSideValue == 1 );
	stop(MAIN_ARM);
}

void fineTune()
{
	// fine tune location of chair
	if( currentBaseValue == 1 )
	{
		// fine tune base
		forward(CHAIR, 125);
		while( currentBaseValue == 1 );
		stop(CHAIR);
		togglePin( LED_PIN, 5, 150 );
	}
	else
	{
		// fine tune base
		backward(CHAIR, 200);
		while( currentBaseValue == 0 );
		stop(CHAIR);
		togglePin( LED_PIN, 4, 350 );
		forward(CHAIR, 150);
		while( currentBaseValue == 1 );
		stop(CHAIR);
	}

}

void togglePin( uint8_t pin, uint8_t repeat, uint16_t duration )
{
	uint8_t i;

	for(i=0; i<repeat; i++)
	{
		digitalWrite(pin, LOW );
		delay( duration );
		digitalWrite( pin, HIGH );
		delay( duration);
	}
	digitalWrite(pin, LOW );
}

/**
 * Initializes the hardware for PWM mode
 *
 */
void initialize()
{
#ifdef __DEBUG
	// Initialize hardware serial port
	Serial.begin(115200);
	Serial.println("INITIALIZING");
#endif

	 // Set I/O to input
	pinMode( LED_PIN, OUTPUT );
	pinMode( BASE_INPUT_PIN, INPUT );
	pinMode( BASE_STATUS_PIN, OUTPUT );

	pinMode( SIDE_INPUT_PIN, INPUT );
	pinMode( SIDE_STATUS_PIN, OUTPUT );

	// Set outputs
	digitalWrite(LED_PIN, LOW);
	digitalWrite( BASE_STATUS_PIN, LOW );
	digitalWrite( SIDE_STATUS_PIN, LOW );

	// Set internal pull up resistor
	digitalWrite(BASE_INPUT_PIN, HIGH);
	digitalWrite(SIDE_INPUT_PIN, HIGH);

	// Setup motors
	uint8_t i, j;
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j< 3; j++ )
		{
			pinMode(motor[i][j], OUTPUT);
			digitalWrite(motor[i][j], LOW);
		}
	}

	// I don't know if this is needed, but since we
	// initialized the pins as digital above, I
	// figure it won't hurt
	analogWrite(motor[0][2], 0);
	analogWrite(motor[1][2], 0);


	// Initialize variables
	led = LOW;
	currentBaseValue = 0;
	currentSideValue = 0;
	baseCount = 0;
	sideCount = 0;
	baseChange = false;
	sideChange = false;

	// Initialize timer (1ms interval)
	FlexiTimer2::set(1, sample);

	// Start the timer
	FlexiTimer2::start();

#ifdef __DEBUG
	Serial.println("INITIALIZED");
#endif

} // end initialize

/**
 * Steps the output from initial setting n times with specified delay
 * between, where n = repeatValue.  Value set to endValue upon completion
 */
void step(uint8_t motorIndex, int8_t direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue)
{

	uint16_t i = 0;

	int8_t speed = initialSpeed;

	for(i=0; i<repeatValue; i++)
	{
		if(direction == FORWARD)
		{
			forward( motorIndex, speed );

		} else if( direction == BACKWARD)
		{
			backward( motorIndex, speed);

		}
		delay( delayValue );
		speed += speedStep;

	}

	if(direction == FORWARD)
	{
		forward( motorIndex, endSpeed );

	} else if( direction == BACKWARD)
	{
		backward( motorIndex, endSpeed );
	}


}

void moveTimed(uint8_t direction, uint8_t motorIndex, uint8_t speed, uint16_t duration)
{
	move( direction, motorIndex, speed );
	delay( duration);
	stop( motorIndex );
}

/**
 * Sets output in specific direction and speed.
 *
 */
void move( uint8_t direction, uint8_t motorIndex, uint8_t speed)
{
	if( direction == FORWARD )
	{
		digitalWrite(motor[motorIndex][0], HIGH);
		digitalWrite(motor[motorIndex][1], LOW);
		analogWrite( motor[motorIndex][2], speed );
	}
	else if( direction == BACKWARD )
	{
		digitalWrite(motor[motorIndex][0], LOW);
		digitalWrite(motor[motorIndex][1], HIGH);
		analogWrite( motor[motorIndex][2], speed );
	}
}

void setSpeed(uint8_t motorIndex, uint8_t speed)
{
	analogWrite( motor[motorIndex][2], speed );
}

void forward(uint8_t motorIndex, uint8_t speed)
{
	digitalWrite(motor[motorIndex][0], HIGH);
	digitalWrite(motor[motorIndex][1], LOW);
	analogWrite( motor[motorIndex][2], speed );
}

void backward(uint8_t motorIndex, uint8_t speed)
{
	digitalWrite(motor[motorIndex][0], LOW);
	digitalWrite(motor[motorIndex][1], HIGH);
	analogWrite( motor[motorIndex][2], speed );
}

void stop(uint8_t motorIndex)
{
	digitalWrite(motor[motorIndex][0], LOW);
	digitalWrite(motor[motorIndex][1], LOW);
	analogWrite( motor[motorIndex][2], 0 );
}


void testOutput()
{
	step(0, FORWARD, 0, 255, 1, 1, 255);
	step(1, FORWARD, 0, 255, 1, 1, 255);
	step(0, FORWARD, 255, 0, -1, 1, 255);
	step(1, FORWARD, 255, 0, -1, 1, 255);
	step(0, BACKWARD, 0, 255, 1, 1, 255);
	step(1, BACKWARD, 0, 255, 1, 1, 255);
	step(0, BACKWARD, 255, 0, -1, 1, 255);
	step(1, BACKWARD, 255, 0, -1, 1, 255);

}

/**
 * Samples the switch input and writes status to status pins
 */
void sample()
{
	debounceInput( BASE_INPUT_PIN, (uint8_t *)&currentBaseValue, (uint8_t *)&baseCount, (uint8_t *)&baseChange );
	debounceInput( SIDE_INPUT_PIN, (uint8_t *)&currentSideValue, (uint8_t *)&sideCount, (uint8_t *)&sideChange );

	digitalWrite( BASE_STATUS_PIN, currentBaseValue );
	digitalWrite( SIDE_STATUS_PIN, currentSideValue );

}

/**
 * Debounces the input
 *
 */
void debounceInput( uint8_t pin, volatile uint8_t *currentValue, volatile uint8_t *currentCount, volatile uint8_t *change )
{
	uint8_t input = 0;
	input = digitalRead(pin);

    if( (input == *currentValue) && (*currentCount > 0) )
    {
    	*currentCount = *currentCount-1;
    }

    if(input != *currentValue)
    {
    	*currentCount = *currentCount + 1;
    }

    // If the Input has shown the same value for long enough let's switch it
    if(*currentCount >= DEBOUNCE_DELAY )
    {
    	// Trap transitions from high to low
    	if( *currentValue == 1 && input == 0 )
    	{
    		*change = true;
    	}
    	*currentCount = 0;
    	*currentValue = input;
    }
}


extern "C" void __cxa_pure_virtual()
{
	cli();
	for (;;);
}

