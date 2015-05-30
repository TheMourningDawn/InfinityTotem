//TODO: Impletment fadeValue
void meteorChaser(bool animate, uint8_t tailLength, uint16_t meteorBodyPixel, uint16_t fadeValue, bool rainbowTail) {
	uint16_t i;
	int meteorHue = random8();
	int fadeSpectrum = fadeValue;
	int fadeIncrement = (256 - fadeValue) / tailLength;

	colorCounter = random8();
	for (uint16_t j = 0; j < NUM_INFINITY_LED; j++) {
		infinity[j].setHue(colorCounter);
	}
	for (i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
		infinity[i].setHue(meteorHue);
		infinity[i].fadeLightBy(fadeSpectrum);
		if (animate == true) {
			FastLED.show();
			delay(20);
		}
		fadeSpectrum += fadeIncrement;
	}
	FastLED.show();
	//Do an optional rainbow tail? Just use rainbow fill on the tail section instead.
}


void wipeRainbow(int delayTime) {
	for (uint8_t i = 0; i <= NUM_INFINITY_LED; i++) {
		fill_rainbow(&(infinity[0]), i, 0);
		FastLED.show();
		delay(delayTime);
	}
}

void wipeInfinity(int delayTime) {
	checkColorCounter(colorCounter, true);
	for (uint8_t i = 0; i < NUM_INFINITY_LED; i++) {
		setPixel(infinity, i);
		FastLED.show();
		delay(delayTime);
		colorCounter += 5;
	}
}
