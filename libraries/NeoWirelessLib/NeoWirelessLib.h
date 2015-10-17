// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _NeoWirelessLib_H_
#define _NeoWirelessLib_H_
#include "Arduino.h"
#include <EEPROM.h>

#define CONFIG_START_ADDRESS	0x10
#define NODE_START_ADDRESS		0x20
#define CLIENT_CONFIG_V10		0x01
#define SERVER_CONFIG_V10		0x01

// Basic Functions
#define CMD_FILL                0x01
#define CMD_FILL_PATTERN        0x02

// Basic Animations
#define CMD_PATTERN 			0x11
#define CMD_WIPE			    0x12
#define CMD_BOUNCE				0x13
#define CMD_MIDDLE				0x14
#define CMD_RANDOM_FLASH        0x15

// Advanced Functions
#define CMD_RAINBOW             0x20
#define CMD_RAINBOW_FADE        0x21
#define CMD_CONFETTI            0x22
#define CMD_CYLON               0x23
#define CMD_BPM                 0x24
#define CMD_JUGGLE              0x25

// Administrative Functions
#define CMD_SET_FPS             0x30
#define CMD_SET_HUE_UPDATE_TIME 0x31

// Other "commands"
#define CMD_ERROR               0x5F

//#define NUM_COMMANDS			9
//static const uint8_t COMMANDS[NUM_COMMANDS] = {CMD_PATTERN, CMD_RUNWAY, CMD_BOUNCE, CMD_MIDDLE, CMD_RAINBOW_FADE, CMD_RAINBOW_FADE_ONE, CMD_RANDOM_FLASH, CMD_RANDOM_FLASH_COLOR, CMD_SET_ALL};
//static const uint8_t COMMAND_LEN[NUM_COMMANDS] = {18, 20, 20, 19, 4, 5, 7, 9};
#define MAX_COMMAND_SIZE		27


//	void setFramesPerSecond(uint8_t fps);

typedef struct ClientConfiguration
{
	uint8_t version;
	uint8_t nodeId;
	uint8_t crc;
} client_configuration_t;
//static client_configuration_t *gClientConfiguration;

typedef struct ServerConfiguration
{
	uint8_t version;
	uint8_t numberNodes;
	uint8_t crc;
} server_configuration_t;
static server_configuration_t *gServerConfiguration;


//	void setFramesPerSecond(uint8_t fps);
typedef struct CommandSetFPS
{
	uint8_t command;
	uint8_t fps;
} set_fps_t;
static set_fps_t *cmdSetFPS;

//	void setHueUpdateTime(uint8_t updateTime);
typedef struct CommandSetHueUpdateTime
{
	uint8_t command;
	uint8_t updateTime;
} set_hue_update_time_t;
static set_hue_update_time_t *cmdSetHueUpdateTime;


//void fill(CRGB color, uint8_t show);
typedef struct CommandFill
{
	uint8_t command;
	uint32_t color;
} fill_t;
static fill_t *cmdFill;

//void fillPattern(uint8_t pattern, CRGB onColor, CRGB offColor);
typedef struct CommandFillPattern
{
	uint8_t command;
	uint8_t pattern;
	uint32_t onColor;
	uint32_t offColor;
} fill_pattern_t;
static fill_pattern_t *cmdFillPattern;


//void pattern(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime);
typedef struct CommandPattern
{
	uint8_t command;
	uint16_t repeat;
	uint8_t pattern;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
} pattern_t;
static pattern_t *cmdPattern;

//void wipe(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
typedef struct CommandWipe
{
	uint8_t command;
	uint8_t pattern;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint8_t clearAfter;
	uint8_t clearEnd;
} wipe_t;
static wipe_t *cmdWipe;

//void bounce(uint8_t pattern, uint8_t direction, CRGB onColor, CRGB offColor, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd);
typedef struct CommandBounce
{
	uint8_t command;
	uint16_t repeat;
	uint8_t pattern;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint32_t bounceTime;
	uint8_t clearAfter;
	uint8_t clearEnd;
} bounce_t;
static bounce_t *cmdBounce;


//void middle(uint8_t direction, CRGB color1, CRGB color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
typedef struct CommandMiddle
{
	uint8_t command;
	uint16_t repeat;
	uint8_t direction;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
	uint8_t clearAfter;
	uint8_t clearEnd;
} middle_t;
static middle_t *cmdMiddle;

//void randomFlash(uint32_t onTime, uint32_t offTime, CRGB onColor, CRGB offColor);
typedef struct CommandRandomFlash
{
	uint8_t command;
	uint32_t onColor;
	uint32_t offColor;
	uint32_t onTime;
	uint32_t offTime;
} random_flash_t;
static random_flash_t *cmdRandomFlash;


//void rainbow(uint8_t glitterProbability);
typedef struct CommandRainbow
{
	uint8_t command;
	uint8_t glitterProbability;
	uint32_t glitterColor;
} rainbow_t;
static rainbow_t *cmdRainbow;

//void rainbowFade();
typedef struct CommandRainbowFade
{
	uint8_t command;
} rainbow_fade_t;
static rainbow_fade_t *cmdRainbowFade;

//void confetti(CRGB color, uint8_t numOn);
typedef struct CommandConfetti
{
	uint8_t command;
	uint32_t color;
	uint8_t numOn;
} confetti_t;
static confetti_t *cmdConfetti;

//void cylon(CRGB color);
typedef struct CommandCylon
{
	uint8_t command;
	uint16_t repeat;
	uint32_t color;
} cylon_t;
static cylon_t *cmdCylon;


//void bpm();
typedef struct CommandBeatsPerMinute
{
	uint8_t command;
} bpm_t;
static bpm_t *cmdBPM;


//void juggle();
typedef struct CommandJuggle
{
	uint8_t command;
} juggle_t;
static juggle_t *cmdJuggle;



uint8_t CRC8(const uint8_t *data, uint8_t len);
uint8_t writeClientConfiguration(client_configuration_t *config);
uint8_t readClientConfiguration(client_configuration_t *config);
void dumpClientConfiguration(client_configuration_t *config);




//Do not add code below this line
#endif /* _NeoWirelessLib_H_ */
