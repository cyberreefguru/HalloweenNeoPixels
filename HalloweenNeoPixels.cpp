// Do not remove the include below
#include "HalloweenNeoPixels.h"

#define NUM_PIXELS 50
#define HALF NUM_PIXELS/2
#define PIN 2
#define DOWN 	0
#define UP		1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN);

uint32_t buffer[NUM_PIXELS];
uint8_t mode = 0;

void selectCrossover();
void fade(uint8_t direction, uint32_t time, uint32_t color);
void strobe(uint32_t duration);
void lightning();
void writeColor(uint32_t color, uint8_t show);


//The setup function is called once at startup of the sketch
void setup()
{
	randomSeed(analogRead(0));
	pixels.begin();
	pixels.setBrightness(255);
	writeColor(WHITE, false);
}

// The loop function is called in an endless loop
void loop()
{
	fade(UP, 10, ORANGE);
	delay(5000);
	fade(DOWN, 10, ORANGE);

	selectCrossover();

	fade(UP, 10, PURPLE);
	delay(5000);
	fade(DOWN, 10, PURPLE);

	fade(UP, 10, RED);
	delay(5000);
	selectCrossover();
	fade(DOWN, 10, RED);

	fade(UP, 10, GREEN);
	delay(5000);
	fade(DOWN, 10, GREEN);
	selectCrossover();

	fade(UP, 10, BLUE);
	delay(5000);
	selectCrossover();
	fade(DOWN, 10, BLUE);

	selectCrossover();

}


void selectCrossover()
{
	uint32_t x, high, low;

	x = random(0,100);
	high = 85;
	low= 20;

	if( x > high )
	{
		lightning();
	}
	else if( x < low )
	{
		strobe(random(750,2000));
	}
}

void fade(uint8_t direction, uint32_t time, uint32_t color)
{
	uint8_t i;

	for(i=0; i<255; i++)
	{
		if( direction == DOWN )
		{
			pixels.setBrightness(255-i);
		}
		else if( direction == UP)
		{
			pixels.setBrightness(i);
		}
		writeColor( color, true );
		delay(time);
	}

}

void strobe(uint32_t duration)
{
	uint32_t end = millis() + duration;
	pixels.setBrightness(255);

	while( millis() < end )
	{
		writeColor(WHITE, true);
		delay(35);
		writeColor(BLACK, true);
		delay(35);
	}
}


void lightning()
{

	uint32_t count, large;
	uint8_t i, b;

	b = false;
	pixels.setBrightness(255);

	count = random(2, 6);
	for(i=0; i<count; i++)
	{
		large = random(0,100);
		writeColor(WHITE, true);
		if( large > 40 && b == false)
		{
			delay(random(100, 350));
			b = true;
		}
		else
		{
			delay(random(20, 50));
		}
		writeColor(BLACK, true);
		if( large > 40 && b == false )
		{
			delay(random(200, 500));
		}
		else
		{
			delay(random(30, 70));
		}
	}
}

void writeColor(uint32_t color, uint8_t show)
{
	for(uint8_t i=0; i<NUM_PIXELS; i++)
	{
		pixels.setPixelColor(i, color);
	}
	if(show)
	{
		pixels.show();
	}
}

