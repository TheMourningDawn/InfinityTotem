/*
 Methods to change the state of the strip
 */

void setStrip(CRGB strip[], CRGB color) {

}

void setStrip(CRGB strip[]) {

}

void setPixel(CRGB strip[], uint16_t index) {
	if (useColorSensor == true) {
		strip[index].setRGB(gammatable[(int) r], gammatable[(int) g], gammatable[(int) b]);
	} else {
		strip[index].setHue(colorCounter);
	}
}

void setPixel(CRGB strip[], uint16_t index, CRGB color) {
	strip[index] = color;
}

//Check out the memmove function to maybe do it more quickly
void shift(CRGB strip[], bool changeDirection) {
	CRGB wrapAroundPixel;
	if (changeDirection == true) {
		wrapAroundPixel = strip[NUM_INFINITY_LED - 1];
		for (int i = NUM_INFINITY_LED - 1; i > 0; i--) {
			strip[i] = strip[i - 1];
		}
		strip[0] = wrapAroundPixel;
	} else {
		wrapAroundPixel = strip[0];
		for (int i = 0; i < NUM_INFINITY_LED; i++) {
			strip[i] = strip[i + 1];
		}
		strip[NUM_INFINITY_LED - 1] = wrapAroundPixel;
	}
}

int runnerPixel = 0; //uint8 signed or unsigned?
void paint(CRGB strip[], CRGB color, bool changeDirection) {
	if (runnerPixel == NUM_INFINITY_LED) { //using this constant wont jive with other strips...
		runnerPixel = 0;
	}
	if (runnerPixel < 0) {
		runnerPixel = NUM_INFINITY_LED;
	}
	strip[runnerPixel] = color;
	if (changeDirection == true) {
		runnerPixel++;
	} else {
		runnerPixel--;
	}
}

void cycleSolid() {
	colorCounter++;
	checkColorCounter(colorCounter);
	for (int i = 0; i < NUM_INFINITY_LED; i++) {
		infinity[i].setHue(colorCounter);
	}
}

//do multiple like it says, dick.
void blinkRandom(uint8_t numberRandomPixels, bool randomColor) {
	uint8_t randomPixel = random8(60);
	uint8_t randomHue = random8();
	for (int i = 0; i < NUM_INFINITY_LED; i++) {
		infinity[i] = CRGB::Black;
		if(randomPixel == i) {
			if(randomColor == true) {
				infinity[i].setHue(colorCounter);
			} else {
				infinity[i].setHue(randomHue);
			}
		}
	}
}
