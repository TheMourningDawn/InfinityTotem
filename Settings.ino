void cycleSettingsMode() {
	settingMode++;
	if (settingMode > 3) {
		settingMode = 0;
	}
	displaySettingMode();
	currentEncoderValue = 0;
	previousEncoderValue = currentEncoderValue; //this is dumb
}

void displaySettingMode() {
	switch (settingMode) {
	case 0:
		setSettingStrip(CRGB::Black, 5);
		settings[0].setHue(15);
		FastLED.show();
		break;
	case 1:
		setSettingStrip(CRGB::Black, 5);
		settings[1].setHue(65);
		FastLED.show();
		break;
	case 2:
		setSettingStrip(CRGB::Black, 5);
		settings[2].setHue(115);
		FastLED.show();
		break;
	case 3:
		setSettingStrip(CRGB::Black, 5);
		settings[3].setHue(165);
		FastLED.show();
		break;
	}
}

void flashSettingsStrip(CRGB color, uint8_t flashDelay, uint8_t flashRepeat) {
	setSettingStrip(color, 5);
	FastLED.show();
	for (uint8_t i = 0; i < flashRepeat; i++) {
		delay(flashDelay);
		setSettingStrip(CRGB::Black, 5);
		FastLED.show();
		delay(flashDelay);
		setSettingStrip(color, 5);
		FastLED.show();
		Serial.println(flashRepeat);
	}
}

void setSettingStrip(CRGB color, uint8_t numberOfDots) {
	for (int i = 0; i < numberOfDots; i++) {
		settings[i] = color;
	}
}

void flashSettingsStripRainbow(uint8_t delay) {

}
