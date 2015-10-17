/*
 * NeoPixelWrapper.h
 *
 *  Created on: Sep 12, 2015
 *      Author: tsasala
 */

#ifndef NEOPIXELWRAPPER_H_
#define NEOPIXELWRAPPER_H_

#include <Arduino.h>
#include "NeoPixelLib.h"
#include <FastLed.h>

#define LED_PIN		2

#define WHITE	CRGB::White
#define BLACK	CRGB::Black
#define RED		CRGB::Red
#define BLUE	CRGB::Blue
#define GREEN	CRGB::Green
#define MAGENTA	CRGB::Magenta
#define CYAN	CRGB::Cyan
#define YELLOW	CRGB::Yellow
#define ORANGE	CRGB::Orange
#define PURPLE	CRGB::Purple
#define RAINBOW	CRGB::Black

class NeoPixelWrapper
{
public:
	NeoPixelWrapper();
	boolean initialize(uint8_t numLeds, uint8_t ledPin);
	void setFramesPerSecond(uint8_t fps);
	void setHueUpdateTime(uint8_t updateTime);


	void rainbow(uint8_t glitterProbability);
	void confetti(CRGB color);
	void sparkle(CRGB color, uint8_t sparkles);
	void cylon(CRGB color);
	void bpm();
	void juggle();
	void fadeRainbow();

	void setPattern(uint8_t pattern, CRGB onColor, CRGB offColor);

	void pattern(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime);
	void rainbowFadeStrip(uint32_t fadeTime);
	void rainbowFadeOne(uint32_t fadeTime);
	void randomFlash(uint32_t onTime, uint32_t offTime);
	void randomFlashColor(uint32_t onTime, uint32_t offTime, CRGB onColor, CRGB offColor);
	void middle(uint8_t direction, CRGB color1, CRGB color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
	void colorBounce(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd);
	void outAndBack(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint32_t bounceTime);
	void colorWipe(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);

	boolean commandDelay(uint32_t waitTime);

protected:
	CRGB *leds;
	void setWipeColor(CRGB newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter);
	void writeColor(CRGB color, uint8_t show);


};

//end of add your includes here
#ifdef __cplusplus
extern "C"
{
#endif

extern boolean isCommandAvailable();

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* NEOPIXELWRAPPER_H_ */
