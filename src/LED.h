#ifndef LED_H
#define LED_H

#include <FastLED.h>

#define NUM_LEDS 1

class LED {
  CRGB leds[NUM_LEDS];

public:
  LED() {
  }

  void setup() {
    FastLED.addLeds<NEOPIXEL, PIN_NEOPIXEL>(leds, NUM_LEDS);
  }

  void setColor(CRGB color) {
    leds[0] = color;
    FastLED.show();
  }
} led;

extern LED led;

#endif