/*
 * These patterns do all of their operations within the function
 * this is bad because it'll be delaying the main loop until it ends
 * settings will not chanage right away, color sensor wont update, etc
 * though, most of these last 1-2 seconds.
 */

void halfTopBottom(bool animate, uint16_t animationDelay, CRGB colorTop, CRGB colorBottom) {
	Serial.println("Starting function");
	uint16_t rightAnchor = 15;
	uint16_t leftAnchor = 45;
	uint16_t endPoint = 45;

	while (rightAnchor < endPoint) {
		setPixel(infinity, rightAnchor, colorTop);
		setPixel(infinity, leftAnchor, colorBottom);
		rightAnchor++;
		leftAnchor++;
		if (leftAnchor == NUM_INFINITY_LED) {
			leftAnchor = 0;
		}
		if (animate == true) {
			FastLED.show();
			delay(animationDelay);
		}
	}
	if (animate == false) {
		FastLED.show();
	}
}


void middleFanout(int delayTime) {
	uint8_t left, right;
	getColorSensorData();
	if (flipFlopState == true) {
		left = 0;
		right = NUM_INFINITY_LED - 1;
		while (left < 30 && right > 30) {
			setPixel(infinity, left);
			setPixel(infinity, right);
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
			setPixel(infinity, left);
			setPixel(infinity, right);
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
	getColorSensorData();
	if (changeChaseDirection == true) {
		for (i = 0; i < NUM_INFINITY_LED; i++) {
			setPixel(infinity, i);
			FastLED.show();
			delay(delayTime);
		}
	} else {
		for (i = NUM_INFINITY_LED; i >= 1; --i) {
			setPixel(infinity, i - 1);
			FastLED.show();
			delay(delayTime);
		}
	}
	colorCounter += 33;
}

void chasingFromSides(bool changeChaseDirection, int delayTime) {
	uint8_t leftTop, rightTop, leftBottom, rightBottom;
	getColorSensorData();
	leftTop = 45; //45 -> 30
	leftBottom = 45; //45 -> 60
	rightTop = 15; //15 -> 0
	rightBottom = 15; //15 -> 30
	while (leftBottom < 60 && leftTop >= 30 && rightBottom < 30 && rightTop >= 0) {
		setPixel(infinity, leftTop);
		setPixel(infinity, leftBottom);
		setPixel(infinity, rightTop);
		setPixel(infinity, rightBottom);
		FastLED.show();
		leftTop--;
		leftBottom++;
		rightTop--;
		rightBottom++;
		delay(delayTime);
	}
	setPixel(infinity, leftTop);
	setPixel(infinity, rightTop);

	colorCounter += 33;
}
