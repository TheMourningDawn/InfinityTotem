void cycleSettingsMode() {
	settingMode++;
	if (settingMode > 3) {
		settingMode = 0;
	}
	displaySettingMode();
	currentEncoderValue = 0;
}

void displaySettingMode() {
	if (settingMode == 0) {
		flashSettingsStrip(CRGB::Blue, 10, 2);
	} else if (settingMode == 1) {
		flashSettingsStrip(CRGB::Green, 10, 2);
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
