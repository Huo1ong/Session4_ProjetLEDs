#include "Adafruit_NeoPixel.h"

#define LED_PIN 12
#define LED_COUNT 10
#define LED_BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS);
  strip.show();
}

void loop() {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); //Couleur rouge
    strip.show();
    delay(100);
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0)); //Couleur verte
    strip.show();
    delay(100);
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); //Couleur bleue
    strip.show();
    delay(100);
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 255)); //Couleur violette
    strip.show();
    delay(100);
  }
}
