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
ClickEncoder *encoder;

// our RGB -> eye-recognized gamma color
byte gammatable[256];

CRGB infinity[NUM_INFINITY_LED];
CRGB settings[NUM_SETTING_LED];

bool flipFlopState = true;
uint16_t colorCounter = 0;
bool useColorSensor = false;

int16_t last, value;

// 0 = select pattern
// 1 = select speed
// 2 = select brightness
// 3 = select color sensor on/off
uint8_t settingMode = 0;
uint8_t showSpeed = 20;

// rgb to use for calcualted color sensor values functions will use
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
	encoder = new ClickEncoder(0, 1, 6, 2, false);
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);

	//Initial value for the 'last' encoder value
	last = -1;

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

	// thanks PhilB for this gamma table!
	// it helps convert RGB colors to what humans see
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
	value += encoder->getValue();

	ClickEncoder::Button b = encoder->getButton();
	if (b != ClickEncoder::Open) {
		if (ClickEncoder::Pressed) {
			settingMode++;
			if (settingMode > 3) {
				settingMode = 0;
			}
		}
	}

	if (value != last) {

		//Why wont a loop work here? WTF
		settings[0] = CRGB::Black;
		settings[1] = CRGB::Black;
		settings[2] = CRGB::Black;
		settings[3] = CRGB::Black;
		settings[4] = CRGB::Black;

		if (settingMode == 0) {

		}

		switch (settingMode) {
		case 0:
			if (value <= 4) {
				settings[0].setHue(0);
				colorCounter = random8();
				for (uint16_t j = 0; j < NUM_INFINITY_LED; j++) {
					infinity[j].setHue(colorCounter);
				}
				FastLED.show();
				meteorChaser(true, 14, 20, 160, true);
			}
			if (value >= 8) {
				wipeRainbow(25);
				settings[1].setHue(45);
			}
			if (value >= 12) {
				settings[2].setHue(90);
			}
			if (value >= 16) {
				settings[3].setHue(135);
				//			for (i = 0; i < NUM_INFINITY_LED; i++) {
				//				settings[i] = CRGB::Black;
				//			}
			}
			if (value > 20) {
				settings[4].setHue(180);
			}
			if (value > 30) {
				settings[5].setHue(215);
			}
			FastLED.show();
			Serial.print("Encoder Value: ");
			Serial.println(value);

			last = value;
			break;
		case 1:
			if (value > last) {
				showSpeed++;
			} else {
				showSpeed--;
			}
			break;
		case 2:
			break;
		default:
			;
		}

	}

	if (settingMode == 0) {
		shift(infinity, true);
		FastLED.show();
		delay(showSpeed);
	}

	if (b != ClickEncoder::Open) {
		Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
		switch (b) {
		VERBOSECASE(ClickEncoder::Pressed)
			;
		VERBOSECASE(ClickEncoder::Held)
		VERBOSECASE(ClickEncoder::Released)
		VERBOSECASE(ClickEncoder::Clicked)
		case ClickEncoder::DoubleClicked:
			Serial.println("ClickEncoder::DoubleClicked");
			encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
			Serial.print("  Acceleration is ");
			Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
			break;
		}
	}

//	colorCounter = random8();
//	for (int i = 0; i < NUM_INFINITY_LED; i++) {
//		infinity[i].setHue(colorCounter);
//	}
//	FastLED.show();
//	meteorChaser(true, 14, 20, 160, true);
//	for (int i = 0; i < 500; i++) {
//	shift(infinity, true);
//	FastLED.show();
//	delay(20);
//	}

//	wipeRainbow(25);
//	halfTopBottom(true, 23, infinity[15], infinity[45]);
//	for (int i = 0; i < NUM_INFINITY_LED; i++) {
//		shift(infinity, false);
//		FastLED.show();
//		delay(20);
//	}
//
//	chasingInfinity(true, 10);
//	chasingInfinity(true, 20);
//	chasingFromSides(false, 45);
//	chasingInfinity(true, 50);
//	chasingFromSides(false, 30);
//	chasingInfinity(false, 10);
//	chasingInfinity(false, 20);
//	chasingInfinity(false, 50);
//	doubleSymmetricalFlipFlow(30);
//	chasingFromSides(false, 30);
//	doubleSymmetricalFlipFlow(15);
//	chasingFromSides(false, 30);
//	doubleSymmetricalFlipFlow(15);
//	doubleSymmetricalFlipFlow(30);
//	doubleSymmetricalFlipFlow(30);
//	doubleSymmetricalFlipFlow(15);
//	checkColorCounter(colorCounter, true);
//
//	wipeRainbow(25);
//	while (delMe < 750) {
//		rainbowShift(10, false, colorCounter);
//		delMe++;
//	}
}

void wipeRainbow(int delayTime) {
	for (uint8_t i = 0; i <= NUM_INFINITY_LED; i++) {
		fill_rainbow(&(infinity[0]), i, colorCounter);
		FastLED.show();
		delay(delayTime);
	}
}

void wipeInfinity(int delayTime) {
	checkColorCounter(colorCounter, true);
	for (uint8_t i = 0; i < NUM_INFINITY_LED; i++) {
		setStrip(infinity, i);
		FastLED.show();
		delay(delayTime);
		colorCounter += 5;
	}
}

void middleDoubleSymmetrical() {
	uint8_t left = 0, right = NUM_INFINITY_LED - 1;
	while (left < 30 && right > 30) {
		setStrip(infinity, left);
		setStrip(infinity, right);
		FastLED.show();
		left += 1;
		right -= 1;
		delay(25);
	}
}

void doubleSymmetricalFlipFlow(int delayTime) {
	uint8_t left, right;
	checkColorCounter(colorCounter, true);
	getColorSensorData();
	if (flipFlopState == true) {
		left = 0;
		right = NUM_INFINITY_LED - 1;
		while (left < 30 && right > 30) {
			setStrip(infinity, left);
			setStrip(infinity, right);
			FastLED.show();
			left += 1;
			right -= 1;
			//If it's the last pixel, dont wait to start the next thing.
			if (right != 0) {
				delay(delayTime);
			}
		}
		flipFlop(flipFlopState);
	} else {
		left = 30;
		right = 30;
		while (right > 0 && left < 60) {
			setStrip(infinity, left);
			setStrip(infinity, right);

			FastLED.show();

			left += 1;
			right -= 1;

			//If it's the last pixel, dont wait to start the next thing.
			if (right != 0) {
				delay(delayTime);
			}
		}
		flipFlop(flipFlopState);
	}
	colorCounter += 32;
}

void chasingInfinity(bool changeChaseDirection, int delayTime) {
	uint8_t i;
	checkColorCounter(colorCounter, true);
	getColorSensorData();
	if (changeChaseDirection == true) {
		for (i = 0; i < NUM_INFINITY_LED; i++) {
			setStrip(infinity, i);
			FastLED.show();
			delay(delayTime);
		}
	} else {
		for (i = NUM_INFINITY_LED; i >= 1; --i) {
			setStrip(infinity, i - 1);
			FastLED.show();
			delay(delayTime);
		}
	}
	colorCounter += 33;
}

void chasingFromSides(bool changeChaseDirection, int delayTime) {
	uint8_t leftTop, rightTop, leftBottom, rightBottom;
	checkColorCounter(colorCounter, true);
	getColorSensorData();
	leftTop = 45; //45 -> 30
	leftBottom = 45; //45 -> 60
	rightTop = 15; //15 -> 0
	rightBottom = 15; //15 -> 30
	while (leftBottom < 60 && leftTop >= 30 && rightBottom < 30 && rightTop >= 0) {
		setStrip(infinity, leftTop);
		setStrip(infinity, leftBottom);
		setStrip(infinity, rightTop);
		setStrip(infinity, rightBottom);
		FastLED.show();
		leftTop--;
		leftBottom++;
		rightTop--;
		rightBottom++;
		delay(delayTime);
	}
	setStrip(infinity, leftTop);
	setStrip(infinity, rightTop);

	colorCounter += 33;
}

/*Trying a new approach here. This should just put it into a state that can be moved with shift.*/
/*Need to convert all patterns to this model, it makes it you are never stuck in a method for any period of time*/
void halfTopBottom(bool animate, uint16_t animationDelay, CRGB colorTop, CRGB colorBottom) {
	Serial.println("Starting function");
	uint16_t rightAnchor = 15;
	uint16_t leftAnchor = 45;
	uint16_t endPoint = 45;

	while (rightAnchor < endPoint) {

		Serial.print("LeftAnchor: ");
		Serial.print(leftAnchor);
		Serial.print(" -  RightAnchor: ");
		Serial.println(rightAnchor);

		setStrip(infinity, rightAnchor, colorTop);
		setStrip(infinity, leftAnchor, colorBottom);
		rightAnchor++;
		leftAnchor++;
		if (leftAnchor == NUM_INFINITY_LED) {
			leftAnchor = 0;
		}
		if (animate == true) {
			Serial.println("Justa bout to show");
			FastLED.show();
			delay(animationDelay);
		}
	}
	if (animate == false) {
		FastLED.show();
	}
}

//TODO: Impletment fadeValue
void meteorChaser(bool animate, uint8_t tailLength, uint16_t meteorBodyPixel, uint16_t fadeValue, bool rainbowTail) {
	uint16_t i;
	int meteorHue = random8();
	int fadeSpectrum = fadeValue;
	int fadeIncrement = (256 - fadeValue) / tailLength;
	for (i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
		infinity[i].setHue(meteorHue);
		infinity[i].fadeLightBy(fadeSpectrum);
		if (animate == true) {
			FastLED.show();
			delay(20);
		}
		fadeSpectrum += fadeIncrement;
	}
	//Do an optional rainbow tail? Just use rainbow fill on the tail section instead.
}

/*Helper Methods*/
void getColorSensorData() {
	uint16_t clear, red, green, blue;
	if (useColorSensor == true) {
		// it takes length of the integration time to read a color
		colorSensor.getRawData(&red, &green, &blue, &clear);
		delay(50);

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

