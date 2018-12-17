#include <FastLED.h>

#define NUM_LEDS 60
#define MAX_BRIGHTNESS  255
#define MIN_BRIGHTNESS  64
#define LED_PIN 6

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void growColors() {
  for( int i = 0; i < NUM_LEDS; i++) {
    if (i % 3 == 0) {
      leds[i] = CRGB::Red;
    } else {
      leds[i] = CRGB::Blue;
    }
  }
  FastLED.show();
}

void toggleLights(bool show) {
  if (show) {
    FastLED.setBrightness(MAX_BRIGHTNESS);
  } else {
    FastLED.setBrightness(0);
  }
  growColors();
}

void loop() { 
  while(Serial.available()) {
    String a = Serial.readString();
    Serial.println(a);
    if (a == "lights on") {
      toggleLights(true);
    } else if (a == "lights off") {
      toggleLights(false);
    }
  }
}
