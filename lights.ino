#ifdef LIGHT_MODULE

#include <FastLED.h>

#define NUM_LEDS 60
#define LED_PIN 6

// Define the array of leds
CRGB leds[NUM_LEDS];

int brightness = 0;
bool topOn = false;
String colorMode = "grow";
CRGB color = CRGB::White;

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void disableTopLEDs() {
  leds[59] = CRGB::Black;
  leds[58] = CRGB::Black;
  leds[57] = CRGB::Black;
  leds[56] = CRGB::Black;

  leds[44] = CRGB::Black;
  leds[43] = CRGB::Black;
  leds[42] = CRGB::Black;
  leds[41] = CRGB::Black;
  leds[40] = CRGB::Black;
  leds[39] = CRGB::Black;
  leds[38] = CRGB::Black;
  leds[37] = CRGB::Black;
  leds[36] = CRGB::Black;
  leds[35] = CRGB::Black;
  leds[34] = CRGB::Black;

  leds[22] = CRGB::Black;
  leds[21] = CRGB::Black;
  leds[20] = CRGB::Black;
  leds[19] = CRGB::Black;
  leds[18] = CRGB::Black;
  leds[17] = CRGB::Black;
  leds[16] = CRGB::Black;
  leds[15] = CRGB::Black;
  leds[14] = CRGB::Black;
  leds[13] = CRGB::Black;
  leds[12] = CRGB::Black;
}

void growColors() {
  for( int i = 0; i < NUM_LEDS; i++) {
    if (i % 3 == 0) {
      leds[i] = CRGB::Red;
    } else {
      leds[i] = CRGB::Blue;
    }
  }
}

void customColors() {
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void updateLights() {
  FastLED.setBrightness(brightness);
  if (colorMode == "grow") {
    growColors();  
  } else if (colorMode == "rgb") {
    customColors();
  }
  if (!topOn) {
    disableTopLEDs();
  }
  FastLED.show();
}

void printStatus() {
  Serial.print("Brightness: ");
  Serial.println(brightness);
  Serial.println("Color Mode: " + colorMode);
  Serial.print("Top: ");
  Serial.println(topOn ? "on" : "off");
}

void printHelp() {
  Serial.println("Commands: ");
  Serial.println("lights < on | full | dim | off >");
  Serial.println("lights brightness <0-255>");
  Serial.println("lights color < white | grow >");
  Serial.println("lights color <0-255> <0-255> <0-255>");
  Serial.println("lights < all | bottom >");
}

void loop() { 
  while(Serial.available()) {
    String a = Serial.readString();
    a.trim();
    Serial.println(a);
    if (a == "status") {
      printStatus();
    } else if (a == "help") {
      printHelp();
    } else  if (a == "lights on" || a == "lights full") {
      brightness = 255;
    } else if (a == "lights off") {
      brightness = 0;
    } else if (a == "lights dim") {
      brightness = 128;
    } else if (a == "lights color white") {
      colorMode = "rgb";
      color = CRGB(255, 255, 255);
    } else if (a == "lights color grow") {
      colorMode = "grow";
    } else if (a == "lights all") {
      topOn = true;
    } else if (a == "lights bottom") {
      topOn = false;
    } else if (a.startsWith("lights brightness ")) {
      brightness = a.substring(18).toInt();
    } else if (a.startsWith("lights color ")) {
      String c = a.substring(13);
      int r = c.substring(0, c.indexOf(' ')).toInt();
      int g = c.substring(c.indexOf(' ') + 1, c.lastIndexOf(' ')).toInt();
      int b = c.substring(c.lastIndexOf(' ') + 1).toInt();

      colorMode = "rgb";
      color = CRGB(r, g, b);
    }
    updateLights();
  }
}

#endif
