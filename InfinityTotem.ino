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
uint8_t colorCounter = 0;

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
	Serial.begin(9600);

	//Initialize FastLED for the infinity symbol strip and the settings strip
	FastLED.addLeds<NEOPIXEL, PIN_INFINITY_STRIP>(infinity, NUM_INFINITY_LED);
	FastLED.addLeds<NEOPIXEL, PIN_SETTINGS_STRIP>(settings, NUM_SETTING_LED);

	//Initialize the encoder (knob) and it's interrupt
	encoder = new ClickEncoder(0, 1, 6, 4, false);
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);

	//Initial value for the previous encoder value
	previousEncoderValue = 0;

	//Start the color sensor. We dont care if it isn't working correctly.
	colorSensor.begin();
//	colorSensor.setInterrupt(true);

	//thanks PhilB for this gamma table! it helps convert RGB colors to what humans see
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

	displaySettingMode();
	animationSelectMode();
}
void loop() {
	colorSensor.setInterrupt(true);
	currentEncoderValue += encoder->getValue();

	ClickEncoder::Button b = encoder->getButton();
	if (b != ClickEncoder::Open) {
		switch (b) {
		case ClickEncoder::Clicked:
			cycleSettingsMode();
			break;
		case ClickEncoder::DoubleClicked:
			flipFlop(useColorSensor);
			encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
			break;
		case ClickEncoder::Released:
			flipFlop(direction);
			break;
		default:
			;
		}
	}

//	if b was open, do this, if not dont...maybe stop the jumping on mode change?
	if (b == ClickEncoder::Open) {
		if (currentEncoderValue != previousEncoderValue) {
			changeSettingsMode();
			previousEncoderValue = currentEncoderValue;
		}
	}

	runAnimation();
	FastLED.show();
}

void changeSettingsMode() {
	switch (settingMode) {
	case 0:
		animationSelectMode();
		break;
	case 1:
		//Color select mode
		colorCounter += 5;
		previousEncoderValue = currentEncoderValue; //this is dumb. I'm dumb. Not sure what to do.
		animationSelectMode();
		break;
	case 2:
		speedSelectMode();
		break;
	case 3:
		//Changes the "brightness" value of the whole strip. Not exactly what you'd think...
		if (currentEncoderValue > previousEncoderValue) {
			for (int i = 0; i < NUM_INFINITY_LED; i++) {
				infinity[i] += 5;
			}
		} else if (currentEncoderValue < previousEncoderValue) {
			for (int i = 0; i < NUM_INFINITY_LED; i++) {
				infinity[i] -= 5;
			}
		}
		break;
	default:
		;
	}
}

void animationSelectMode() {
	changeSettingValue();

	switch (settingValue) {
	case 0: //shift
		fill_solid(&(infinity[0]), 60, CRGB::Black);
		meteorChaser(15, 12, 160, false);
		break;
	case 1: //shift
		if (settingMode == 0) {
			fill_solid(&(infinity[0]), 60, CRGB::Black);
		}
		fourPoints(7, 21, 35, 49);
		break;
	case 2: //shift
		colorCounter = random8();
		fillSolid(colorCounter);
		colorCounter = random8();
		meteorChaser(20, 14, 160, true);
		break;
	case 3: //shift
		fillSolid(colorCounter);
		meteorChaser(30, 26, 160, true);
		break;
	case 4: //shift
		wipeInfinity(25);
		break;
	case 5: //cycleSolid
		fillSolid(colorCounter);
		break;
	default:
		break;
	}

	FastLED.show();
}

void runAnimation() {
	switch (settingValue) {
	case 5:
		cycleSolid();
		delay(showSpeed);
		break;
	case 6:
		middleFanout(30);
		break;
	case 7:
		chasingFromSides(false, showSpeed);
		break;
	case 8:
		chasingInfinity(true, showSpeed);
		break;
	case 9:
		blinkRandom(1, true);
		break;
	default:
		shift(infinity, direction);
		delay(showSpeed);
		break;
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

void changeSettingValue() {
	if (currentEncoderValue > previousEncoderValue) {
		settingValue++;
	} else if (currentEncoderValue < previousEncoderValue) {
		settingValue--;
	}
	checkSettingValueBoundries();
}

void checkSettingValueBoundries() {
	if (settingValue >= 254) {
		settingValue = 9;
	}
	if (settingValue > 10) {
		settingValue = 0;
	}
}

void flipFlop(bool &flopToFlip) {
	if (flopToFlip == true) {
		flopToFlip = false;
		return;
	}
	flopToFlip = true;
}

