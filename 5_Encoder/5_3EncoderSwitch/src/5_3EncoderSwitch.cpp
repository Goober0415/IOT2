/* 
 * Project l05_03_encoderSwitch
 * Author: Jamie Gavina
 * Date: 2/6/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */


// Include Particle Device OS APIs
#include "Particle.h"
#include "Encoder.h"
#include "neopixel.h"
#include "Colors.h"
#include "Button.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// Constants
const int PIXELCOUNT = 16;
const int ENCODER_PIN_A = D8;
const int ENCODER_PIN_B = D9;
const int RED_PIN = D5;
const int GREEN_PIN = D4;
//const int BLUE_PIN = D3;

// Component initialization
Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
Button switchBtn(D16);

// State variables
int lastPosition = 0;
unsigned long lastUpdateTime = 0;
bool buttonState = false;


void onOff();

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 1000);
    Serial.println("Ready");

    // Initialize NeoPixel strip
    pixel.begin();
    pixel.setBrightness(10);

    // Configure RGB LED pins
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    //pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    // Button click detection with debounce handling
    static unsigned long lastButtonTime = 0;
    const uint16_t DEBOUNCE_DELAY = 50;

    if (switchBtn.isClicked()) {
        if ((millis() - lastButtonTime) >= DEBOUNCE_DELAY) {
            Serial.println("on/off");
            buttonState = !buttonState;
            lastButtonTime = millis();
        }
    }

    if (buttonState == true) {
        static unsigned long lastUpdateTime = 0;
        const uint16_t UPDATE_INTERVAL = 50;

        if ((millis() - lastUpdateTime) >= UPDATE_INTERVAL) {
            int currentPosition = encoder.read();

            if (currentPosition != lastPosition) {
                Serial.printf("Encoder position: %d\n", currentPosition);
                
                // Value constraints and mapping
                int constrainedPos = constrain(currentPosition, 0, 95);
                int pixelIndex = map(constrainedPos, 0, 95, 0, PIXELCOUNT - 1);

                // Pixel update logic optimized
                for (int i = 0; i < PIXELCOUNT; i++) {
                    pixel.setPixelColor(i, violet);
                }
                pixel.setPixelColor(pixelIndex, blue);
                pixel.show();

                lastPosition = currentPosition;
            }
            
            lastUpdateTime = millis();
        }
    }

    onOff();  // Additional function call
}

void onOff() {
    
    if (switchBtn.isClicked()) {
        // Device is ON
        pixel.clear();                    // Clear NeoPixels
        digitalWrite(RED_PIN, HIGH);      // Show red status
        digitalWrite(GREEN_PIN, LOW);
        //digitalWrite(BLUE_PIN, LOW);
    } else {
        // Device is OFF
        pixel.show();                     // Restore NeoPixel display
        digitalWrite(RED_PIN, LOW);       // Show green status
        digitalWrite(GREEN_PIN, HIGH);
        //digitalWrite(BLUE_PIN, LOW);
    }
}
