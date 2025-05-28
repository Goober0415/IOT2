/* 
 * Project 6_2Keypad
 * Author: Jamie Dowden-Duarte
 * Date: 5/28/25
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Keypad_I2C.h"

SYSTEM_MODE(AUTOMATIC);

const byte ROWS = 4;
const byte COLS = 4;
char customKey;

char hexaKeys[ROWS][COLS] ={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins [ROWS] = {D8,D9,D16,D15};
byte colPins [COLS] = {D17,D18,D19,D14};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);
  waitFor(Serial.isConnected,1000);
}

void loop() {
customKey = customKeypad.getKey();

if (customKey){
  Serial.printf("Key Pressed: %c\n", customKey);
  Serial.printf("Key Pressed (Hex Code) 0x%02%\n", customKey);
 }
}
