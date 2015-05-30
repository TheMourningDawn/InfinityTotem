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
