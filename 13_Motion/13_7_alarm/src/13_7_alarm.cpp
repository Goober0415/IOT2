/*
 * Project alarm
 * Author: Jamie Gavina
 * Date: 11/21/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "neopixel.h"
#include "button.h"

SYSTEM_MODE(AUTOMATIC);

const int PIXELCOUNT = 1;
const int HALL_PIN = A5;   // Pin connected to Hall Effect sensor
const int BUTTON_PIN = D3; // Pin connected to button
int times = 500;

Button WHITE(BUTTON_PIN);
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

bool disabled = true;
bool magnetPresent = false;
bool blink;
uint32_t lastChangeTime = 0;

void blinkLED(uint32_t color);

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 1000);

  pixel.begin();
  pixel.setBrightness(10);
  pinMode(HALL_PIN, INPUT_PULLUP);
}

void loop()
{
  static bool previousState = magnetPresent;

  if (WHITE.isPressed())
  {
    disabled = !disabled;
    pixel.clear();
    pixel.show();
  }

  if (!disabled)
  {
    magnetPresent = digitalRead(HALL_PIN);

    if (magnetPresent && previousState == false)
    {
      blinkLED(0xFF0000); // RED - armed and magnet detected
    }
    else if (!magnetPresent && previousState == true)
    {
      pixel.setPixelColor(0, 0, 255, 0);
      pixel.show();
    }

    previousState = magnetPresent;
  }
  else
  {
    pixel.setPixelColor(0, 0, 0, 255);
    pixel.show();
  }
}

void blinkLED(uint32_t color, int times)
{
  static uint32_t lastTime = 0;

  uint32_t currentTime = millis();

  if (currentTime - lastTime > times)
  {
    lastTime = currentTime;
    blink = !blink;
  }

  if (blink == TRUE)
  {
    pixel.clear();
    pixel.setPixelColor(0, color);
    pixel.show();
  }
}
