/* 
 * Project l05_02_NeoPixel
 * Author: Jamie Gavina
 * Date: 2/6/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <neopixel.h>
#include <Encoder.h>
#include <Colors.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 16;
const int PINA = D8;
const int PINB = D9;

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Encoder myEnc(PINA, PINB);

int currentPosition;
int lastPosition;
unsigned long currentTime = millis();
unsigned long lastUpdateTime;

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);
  Serial.printf("Ready to go\n");
  pixel.begin();
  pixel.setBrightness(10);
}

void loop()
{
  unsigned long currentTime = millis();

  // if (currentTime - lastUpdateTime >= 50)
  // { // Update every 50ms
  currentPosition = myEnc.read();

  if (currentPosition != lastPosition)
  {

    Serial.printf("Encoder position: %i\n", currentPosition);

    lastPosition = currentPosition;
  }

  lastUpdateTime = currentTime;
  static int prevPos = -1;

  int pos = myEnc.read();

  // Bound the encoder value
  pos = constrain(pos, 0, 95);

  // Map encoder position to pixel index
  int pixelIndex = map(pos, 0, 95, 0, PIXELCOUNT - 1);

  // Update pixels
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    pixel.setPixelColor(i, violet);
  }
  pixel.setPixelColor(pixelIndex, blue);

  // Only print if the position has changed
  if (pos != prevPos)
  {
    Serial.print("Encoder position: \n");
    Serial.println(pos);
    prevPos = pos;
  }

  pixel.show();
}
