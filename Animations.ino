void meteorChaser(uint16_t meteorBodyPixel, uint8_t tailLength, uint16_t fadeValue, bool rainbowTail) {
	uint16_t i;
	int fadeSpectrum = fadeValue;
	int fadeIncrement = (256 - fadeValue) / tailLength;

	if (rainbowTail == true) {
		fill_rainbow(&(infinity[meteorBodyPixel]), tailLength, colorCounter);
	} else {
		if (direction == true) {
			for (i = meteorBodyPixel; i > (meteorBodyPixel - tailLength); i--) {
				infinity[i].setHue(colorCounter);
				infinity[i].fadeLightBy(fadeSpectrum);
				fadeSpectrum += fadeIncrement;
			}
		} else {
			for (i = (meteorBodyPixel - tailLength); i < meteorBodyPixel; i++) {
				infinity[i].setHue(colorCounter);
				infinity[i].fadeLightBy(fadeSpectrum);
				fadeSpectrum += fadeIncrement;
			}
		}
	}
	FastLED.show();
}

int bodyPixel = 0; //needs to be signed
void meteorChaserColorSensor(uint8_t tailLength, uint16_t fadeValue) {
	int i;
	int fadeSpectrum = fadeValue;
	int fadeIncrement = (256 - fadeValue) / tailLength;

	getColorSensorData();
	blackInfinityStrip();
	if(bodyPixel > NUM_INFINITY_LED-1){
		bodyPixel = 0;
	} else if(bodyPixel < 0) {
		bodyPixel = NUM_INFINITY_LED-1;
	}
	if (direction == true) {
		for (i = bodyPixel; i > (bodyPixel - tailLength); i--) {
			infinity[correctNumber(i)].setRGB(gammatable[(int) r], gammatable[(int) g], gammatable[(int) b]);
			infinity[correctNumber(i)].fadeLightBy(fadeSpectrum);
			fadeSpectrum += fadeIncrement;
		}
		bodyPixel++;
	} else {
		for (i = (bodyPixel - tailLength); i < bodyPixel; i++) {
			infinity[correctNumber(i)].setRGB(gammatable[(int) r], gammatable[(int) g], gammatable[(int) b]);
			infinity[correctNumber(i)].fadeLightBy(fadeSpectrum);
			fadeSpectrum += fadeIncrement;
		}
		bodyPixel--;
	}
	FastLED.show();
}

//wtf do I even name this?
uint8_t correctNumber(int number) {
	if(number > NUM_INFINITY_LED-1)
		return (number-NUM_INFINITY_LED);
	else if(number < 0)
		return (number+NUM_INFINITY_LED);
	else
		return number;
}

void wipeRainbow(int delayTime) {
	fill_rainbow(&(infinity[0]), NUM_INFINITY_LED, 0);
	FastLED.show();
}

void wipeInfinity(int delayTime) {
	for (uint8_t i = 0; i < NUM_INFINITY_LED; i++) {
		setPixel(infinity, i);
		FastLED.show();
		delay(delayTime);
		colorCounter += 4;
	}
}

void fillSolid(CRGB color) {
	fill_solid(&(infinity[0]), 60, color);
}

void fillSolid(uint16_t colorCounter) {
	for (uint8_t i = 0; i < NUM_INFINITY_LED; i++) {
		infinity[i].setHue(colorCounter);
	}
}

void fourPoints(uint8_t point1, uint8_t point2, uint8_t point3, uint8_t point4) {
	for (uint8_t i = 0; i < NUM_INFINITY_LED; i++) {
		if (i == point1 || i == point2 || i == point3 || i == point4) {
			infinity[i].setHue(colorCounter);
		}
	}
	FastLED.show();
}

////This might get weird
//uint8_t bodyPixel = 0;
//void colorSensorMeteorShift() {
//	int fadeSpectrum = fadeValue;
//	int fadeIncrement = (256 - fadeValue) / tailLength;
//}

//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t Wheel(byte WheelPos) {
//  if(WheelPos < 85) {
//   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//  } else if(WheelPos < 170) {
//   WheelPos -= 85;
//   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  } else {
//   WheelPos -= 170;
//   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//}
