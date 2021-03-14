#include "libRGBW.h"
#include "Arduino.h"

#include <Streaming.h>

constexpr uint8_t PIN_LED_GREEN=5;
constexpr uint8_t PIN_LED_RED=10;
constexpr uint8_t PIN_LED_BLUE=11;
constexpr uint8_t PIN_LED_WHITE=6;

smartRGBW MyRGBW;
uint32_t next_step;

void setup() {
	Serial.begin(230400);
	Serial<<"zaczeto setup\n";
	pinMode(PIN_LED_WHITE, OUTPUT);
	pinMode(PIN_LED_RED, OUTPUT);
	pinMode(PIN_LED_GREEN, OUTPUT);
	pinMode(PIN_LED_BLUE, OUTPUT);
	next_step=0;

}


void loop()
{

	RGB color;
	color.R = analogRead(A1)*0.25;
	color.G = analogRead(A3)*0.25;
	color.B = analogRead(A2)*0.25;
	float intensity = analogRead(A5)/1023.*4.; //procent
	float maxIntensity = MyRGBW.calculateMaxIntensity(color);
//	intensity *= maxIntensity;

	RGBW colorLED = MyRGBW.getRGBW(color, intensity);

	analogWrite(PIN_LED_WHITE, min(255, colorLED.W));
	analogWrite(PIN_LED_RED, min(255, colorLED.R));
	analogWrite(PIN_LED_GREEN, min(255, colorLED.G));
	analogWrite(PIN_LED_BLUE, min(255, colorLED.B));

	if(millis()>next_step) {
		Serial<<"color("<<color.R<<", "<<color.G<<", "<<color.B<<"); i="<<intensity<<", max_i"<<maxIntensity<<"\n";
		Serial<<"RGBI("<<colorLED.R<<", "<<colorLED.G<<", "<<colorLED.B<<"; "<<colorLED.W<<")\n";
		next_step = millis()+500;
	}

}
