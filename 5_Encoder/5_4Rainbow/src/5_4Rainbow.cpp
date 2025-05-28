/* 
 * Project l05_04_rainbow
 * Author: Jamie Gavina
 * Date: 2/07/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <neopixel.h>
#include <Colors.h>
#include <Button.h>
#include <encoder.h>

SYSTEM_MODE(SEMI_AUTOMATIC);

const int PIXELCOUNT = 16;
const int BPIN = D16;

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Button BUTTON(BPIN);

void updateRingColor(int i);

int i;

void setup() {
  pixel.begin();
  pixel.setBrightness(10);

  
}

void loop() {
    // Check for button click
    if(BUTTON.isClicked()) {
      // Cycle through rainbow colors
     i = (i + 1) % 6;
      updateRingColor(i);
    }
  }
  
  void updateRingColor(int i) {
    uint32_t color;
    
    switch(i) {
      case 0: color = pixel.Color(255, 0, 0);     break;  // Red
      case 1: color = pixel.Color(255, 165, 0);   break;  // Orange
      case 2: color = pixel.Color(255, 255, 0);   break;  // Yellow
      case 3: color = pixel.Color(0, 128, 0);     break;  // Green
      case 4: color = pixel.Color(0, 0, 255);     break;  // Blue
      case 5: color = pixel.Color(75, 0, 130);    break;  // Indigo
      default: color = pixel.Color(0, 0, 0);      break;  // Black
    }
    
    // Set all pixels to selected color
    for(int i = 0; i < PIXELCOUNT; i++) {
      pixel.setPixelColor(i, color);
    }
    pixel.show();
  }
