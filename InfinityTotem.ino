#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <FastLED.h>

// set to false if using a common cathode LED
#define commonAnode false

#define PIN 6
#define NUM_LED 60

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

// our RGB -> eye-recognized gamma color
byte gammatable[256];

CRGB strip[NUM_LED];
bool flipFlopState = true;
uint16_t colorCounter = 0;
bool useColorSensor = false;

uint16_t delMe = 0;

// rgb to use for calcualted color sensor values functions will use
float r, g, b;

void setup() {
	Serial.begin(9600);

//Initialize FastLED for the infinity symbol strip
	FastLED.addLeds<NEOPIXEL, PIN>(strip, NUM_LED);

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
	for (int i = 0; i < 256; i++) {
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
	if (delMe >= 750) {
		delMe = 0;
	}

	//Turn the light off
//	colorSensor.setInterrupt(true);

//	for(int i=0;i<NUM_LED;i++){
//		strip[i] = CRGB::Black;
//	}
//	FastLED.show();
//	delay(200);

	halfTopBottom(true, 25, CRGB::Amethyst, CRGB::DarkRed);
	for (int i = 0; i < NUM_LED; i++) {
		shift(true);
		FastLED.show();
		Serial.println("Did done it");
	}
	delay(100);
	halfTopBottom(true, 25, CRGB::SpringGreen, CRGB::LightSalmon);
	for (int i = 0; i < NUM_LED; i++) {
		shift(false);
	}

//	halfTopBottom(true, 30, CRGB::DarkRed, CRGB::Amethyst);
//	delay(100);
//	fill_solid( &(leds[i]), NUM_LED , CRGB::Black);
//	delay(100);

//	colorCounter = 0;
//	wipeRainbow(25);
//	while (delMe < 750) {
//		rainbowShift(10, false, colorCounter);
//		delMe++;
//	}
//
//	colorCounter = random8();
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
}

void wipeRainbow(int delayTime) {
	for (uint8_t i = 0; i <= NUM_LED; i++) {
		fill_rainbow(&(strip[0]), i, colorCounter);
		FastLED.show();
		delay(delayTime);
	}
}

void rainbowShift(int delayTime, bool changeDirection, int startHue) {
	checkColorCounter(colorCounter, true);
	if (startHue != 0) {
		colorCounter = startHue;
	}
	if (changeDirection == true) {
		fill_rainbow(&(strip[0]), NUM_LED, 255 - colorCounter);
	} else {
		fill_rainbow(&(strip[0]), NUM_LED, colorCounter);
	}
	FastLED.show();
	delay(delayTime);
	colorCounter += 1;
}

void wipeInfinity(int delayTime) {
	checkColorCounter(colorCounter, true);
	for (uint8_t i = 0; i < NUM_LED; i++) {
		setStrip(strip, i);
		FastLED.show();
		delay(delayTime);
		colorCounter += 5;
	}
}

void middleDoubleSymmetrical() {
	uint8_t left = 0, right = NUM_LED - 1;
	while (left < 30 && right > 30) {
		setStrip(strip, left);
		setStrip(strip, right);
		FastLED.show();
		left += 1;
		right -= 1;
		delay(25);
	}
}

void doubleSymmetricalFlipFlow(int delayTime) {
	uint8_t left, right;
	checkColorCounter(colorCounter, true);
	getSensorData();
	if (flipFlopState == true) {
		left = 0;
		right = NUM_LED - 1;
		while (left < 30 && right > 30) {
			setStrip(strip, left);
			setStrip(strip, right);
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
			setStrip(strip, left);
			setStrip(strip, right);

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
	getSensorData();
	if (changeChaseDirection == true) {
		for (i = 0; i < NUM_LED; i++) {
			setStrip(strip, i);
			FastLED.show();
			delay(delayTime);
		}
	} else {
		for (i = NUM_LED; i >= 1; --i) {
			setStrip(strip, i - 1);
			FastLED.show();
			delay(delayTime);
		}
	}
	colorCounter += 33;
}

void chasingFromSides(bool changeChaseDirection, int delayTime) {
	uint8_t leftTop, rightTop, leftBottom, rightBottom;
	checkColorCounter(colorCounter, true);
	getSensorData();
	leftTop = 45; //45 -> 30
	leftBottom = 45; //45 -> 60
	rightTop = 15; //15 -> 0
	rightBottom = 15; //15 -> 30
	while (leftBottom < 60 && leftTop >= 30 && rightBottom < 30 && rightTop >= 0) {
		setStrip(strip, leftTop);
		setStrip(strip, leftBottom);
		setStrip(strip, rightTop);
		setStrip(strip, rightBottom);
		FastLED.show();
		leftTop--;
		leftBottom++;
		rightTop--;
		rightBottom++;
		delay(delayTime);
	}
	setStrip(strip, leftTop);
	setStrip(strip, rightTop);

	colorCounter += 33;
}

/*Trying a new approach here. This should just put it into a state that can be moved with shift.*/
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

		setStrip(strip, rightAnchor, colorTop);
		setStrip(strip, leftAnchor, colorBottom);
		rightAnchor++;
		leftAnchor++;
		if (leftAnchor == NUM_LED) {
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

void halfRightLeft(bool animate) {

}

void meteorChaser(bool animate, uint8_t tailLength, uint8_t fadeValue) {

}

/*Helper Methods*/

void setStrip(CRGB strip[], uint16_t index) {
	if (useColorSensor == true) {
		strip[index].setRGB(gammatable[(int) r], gammatable[(int) g], gammatable[(int) b]);
	} else {
		strip[index].setHue(colorCounter);
	}
}

void setStrip(CRGB strip[], uint16_t index, CRGB color) {
	strip[index] = color;
}

void getSensorData() {
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

//TODO: this isn't safe when either parameter is close to either end of the strip
void shift(int to, int from) {
	if (to < from) {
		for (int i = to; i < from; i++) {
			strip[i] = strip[i + 1];
		}
	} else {
		for (int i = to; i > from; i--) {
			strip[i] = strip[i - 1];
		}
	}
}

//Check out the  memmove function to maybe do it more quickly
void shift(bool changeDirection) {
	uint32_t wrapAroundPixel;
	if (changeDirection == true) {
		wrapAroundPixel = strip[NUM_LED - 1];
		for (int i = NUM_LED - 1; i > 0; i++) {
			strip[i] = strip[i - 1];
		}
		strip[0] = wrapAroundPixel;
	} else {
		wrapAroundPixel = strip[0];
		for (int i = 0; i < NUM_LED; i++) {
			strip[i] = strip[i + 1];
		}
		strip[NUM_LED - 1] = wrapAroundPixel;
	}
}

struct Painter {
	byte r;
	byte g;
	byte b;
};
