#include <Wire.h>
#include <FastLED.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include "Adafruit_TCS34725.h"

// set to false if using a common cathode LED
#define commonAnode false

#define PIN_INFINITY_STRIP 12
#define PIN_SETTINGS_STRIP 9

#define PIN_ENCODER_A 0
#define PIN_ENCODER_B 1
#define PIN_ENCODER_SWITCH 6

#define NUM_INFINITY_LED 60
#define NUM_SETTING_LED 5

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
bool useColorSensor = false;

ClickEncoder *encoder;
int16_t previousEncoderValue, currentEncoderValue;

// our RGB -> eye-recognized gamma color
byte gammatable[256];

CRGB infinity[NUM_INFINITY_LED];
CRGB settings[NUM_SETTING_LED];

bool flipFlopState = true;
uint16_t colorCounter = 0;

// 0 = select pattern
// 1 = select speed
// 2 = select brightness
// 3 = select color sensor on/off
// double click = change colors
uint8_t settingMode = 0;
uint8_t settingValue = 0;
uint16_t showSpeed = 24;
bool colorSensorEnabled = false;
bool direction = true;

// rgb to use for calculated color sensor values some functions use
float r, g, b;

void timerIsr() {
	encoder->service();
}

void setup() {
//	Serial.begin(9600);

	//Initialize FastLED for the infinity symbol strip and the settings strip
	FastLED.addLeds<NEOPIXEL, PIN_INFINITY_STRIP>(infinity, NUM_INFINITY_LED);
	FastLED.addLeds<NEOPIXEL, PIN_SETTINGS_STRIP>(settings, NUM_SETTING_LED);

	//Initialize the encoder (knob) and it's interrupt
	encoder = new ClickEncoder(0, 1, 6, 4, false);
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);

	//Initial value for the previous encoder value
	previousEncoderValue = -1;

	//Start the color sensor if necessary
	if (useColorSensor == true) {
		if (colorSensor.begin()) {
			Serial.println("Found sensor");
		} else {
			Serial.println("No TCS34725 found ... check your connections");
			while (1)
				; // halt!
		}
//		colorSensor.setInterrupt(false);
	}

	// thanks PhilB for this gamma table! it helps convert RGB colors to what humans see
	for (uint16_t i = 0; i < 256; i++) {
		float x = i;
		x /= 255;
		x = pow(x, 2.5);
		x *= 255;

		if (commonAnode) {
			gammatable[i] = 255 - x;
		} else {
			gammatable[i] = x;
		}
	}
}

void loop() {
	currentEncoderValue += encoder->getValue();

	ClickEncoder::Button b = encoder->getButton();
	if (b != ClickEncoder::Open) {
		switch (b) {
		case ClickEncoder::Clicked:
			cycleSettingsMode();
			break;
		case ClickEncoder::DoubleClicked:
			encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
			break;
		case ClickEncoder::Released:
			flipFlop(direction);
			break;
		default:
			;
		}
	}

	if (currentEncoderValue != previousEncoderValue) {
		setSettingStrip(CRGB::Black, 5);
		switch (settingMode) {
		case 0:
			animationSelectMode();
			break;
		case 1:
			speedSelectMode();
			break;
		case 2:
			//Will be brightness if possible
			break;
		case 3:
			//Use the color sensor, or not
			break;
		default:
			;
		}
		previousEncoderValue = currentEncoderValue;
	}
	shift(infinity, direction);
	FastLED.show();
	delay(showSpeed);
}

void animationSelectMode() {
	if (currentEncoderValue > previousEncoderValue) {
		settingValue++;
	} else if (currentEncoderValue < previousEncoderValue) {
		settingValue--;
	}
	if (settingValue <= 4) {
		settings[0].setHue(0);
		meteorChaser(true, 14, 20, 160, true);
	} else if (settingValue <= 5) {
		settings[0].setHue(0);
		settings[1].setHue(45);
		wipeInfinity(25); //need a new infinity wipe that doesn't suck
	} else if (settingValue <= 6) {
		settings[0].setHue(0);
		settings[1].setHue(45);
		settings[2].setHue(90);
		doubleSymmetricalFlipFlow(30);
	} else if (settingValue <= 7) {
		settings[0].setHue(0);
		settings[1].setHue(45);
		settings[2].setHue(90);
		settings[3].setHue(135);
		chasingFromSides(false, 30);
	} else if (settingValue <= 8) {
		settings[0].setHue(0);
		settings[1].setHue(45);
		settings[2].setHue(90);
		settings[3].setHue(135);
		settings[4].setHue(180);
		chasingInfinity(true, 10);
	} else if (settingValue >= 9) {
		setSettingStrip(CRGB::Blue, 5);
	}
	FastLED.show();
}

void speedSelectMode() {
	if (currentEncoderValue > previousEncoderValue) {
		showSpeed -= 2;
	} else if (currentEncoderValue < previousEncoderValue) {
		showSpeed += 2;
	}
	if (showSpeed <= 4) {
		showSpeed = 5;
	}
}

/*
 * Puts data from color sensor into the r, g, b global variables
 * Use with gammatable[(int) r] to get a reasonable color
 */
void getColorSensorData() {
	uint16_t clear, red, green, blue;
	if (useColorSensor == true) {
		// it takes length of the integration time to read a color
		colorSensor.getRawData(&red, &green, &blue, &clear);
		delay(50); // this delay could be a parameter or something

		// Figure out some basic hex code for visualization
		uint32_t sum = clear;
		r = red;
		r /= sum;
		g = green;
		g /= sum;
		b = blue;
		b /= sum;
		r *= 256;
		g *= 256;
		b *= 256;
	}
}

//Shouldn't really need this if a uint8 is used. Will just roll over at 255
void checkColorCounter(uint16_t &colorCounter, bool leaveMod) {
	if (colorCounter > 255) {
		if (leaveMod == true) {
			colorCounter = colorCounter % 255;
		} else {
			colorCounter = 0;
		}
	}
}

void flipFlop(bool &flopToFlip) {
	if (flopToFlip == true) {
		flopToFlip = false;
		return;
	}
	flopToFlip = true;
}

