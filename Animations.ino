//TODO: Deal with the body pixel being too close to either end
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
	//Do an optional rainbow tail? Just use rainbow fill on the tail section instead.
}

void wipeRainbow(int delayTime) {
	fill_rainbow(&(infinity[0]), NUM_INFINITY_LED, 0);
	FastLED.show();
}

void wipeInfinity(int delayTime) {
	checkColorCounter (colorCounter);
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

void fourPoints(uint8_t point1, uint8_t point2 ,uint8_t point3 ,uint8_t point4) {
	for(uint8_t i=0;i<NUM_INFINITY_LED;i++){
		if(i == point1 || i == point2 || i == point3 || i == point4) {
			infinity[i].setHue(colorCounter);
		}
	}
	FastLED.show();
}

////Trying this out to see if it wont have a seam...
//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
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
