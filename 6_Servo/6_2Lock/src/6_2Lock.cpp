/* 
 * Project 6_2Lock
 * Author: Jamie Gavina
 * Date: 5/29/25
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */
#include "Particle.h"
#include "Keypad_I2C.h"

SYSTEM_MODE(AUTOMATIC);

// Define constants for keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
unsigned int currentTime = millis();
unsigned int lastTime;

// Define the keypad layout
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define pins for keypad connection
byte rowPins[ROWS] = {D8,D9,D16,D15};
byte colPins[COLS] = {D17,D18,D19,D14};

// Initialize keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Define arrays for storing codes
const char MASTER_CODE[] = "1234";  // Default master code
char enteredCode[4];                 // Array to store entered digits
int codeIndex = 0;                   // Track position in entered code

// Define pins for outputs
const int SERVO_PIN = A2;
const int RED_LED_PIN = D7;
const int GREEN_LED_PIN = D6;

Servo myServo;

bool compareCodes(const char* code1, const char* code2, int length);

void setup() {
    Serial.begin(9600);
    waitFor(Serial.isConnected, 1000);
    
    // Initialize servo
    myServo.attach(SERVO_PIN);
    myServo.write(180);  // Start in locked position
    
    // Initialize LEDs as outputs
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    
    // Show initial locked state
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
}

void loop() {
    char customKey = customKeypad.getKey();
    
    if(customKey && customKey != '*' && customKey != '#') {
        // Store entered digit
        enteredCode[codeIndex++] = customKey;
        Serial.printf("Digit entered: %c\n", customKey);
        
        // Check if we have a complete 4-digit code
        if(codeIndex == 4) {
            Serial.println("Complete code entered");
            
            // Compare entered code with master code
            if(compareCodes(enteredCode, MASTER_CODE, 4)) {
                Serial.println("Correct code!");
                // Unlock sequence
                myServo.write(0);
                digitalWrite(RED_LED_PIN, LOW);
                digitalWrite(GREEN_LED_PIN, HIGH);
                
                // Auto-lock after 5 seconds
               if((currentTime - lastTime)>5000){
                myServo.write(180);
                digitalWrite(RED_LED_PIN, HIGH);
                digitalWrite(GREEN_LED_PIN, LOW);
                currentTime = lastTime;
               }
            } else {
                Serial.println("Incorrect code!");
                // Flash red LED three times
                for(int i = 0; i < 3; i++) {
                    digitalWrite(RED_LED_PIN, LOW);
                    delay(200);
                    digitalWrite(RED_LED_PIN, HIGH);
                    delay(200);
                }
            }
            
            // Reset entered code array
            codeIndex = 0;
        }
        
        // Clear keypad buffer
        customKeypad.getKey();
    }
    
    // Handle clear (*) key
    if(customKey == '*') {
        codeIndex = 0;
        Serial.println("Code entry cleared");
    }
}

bool compareCodes(const char* code1, const char* code2, int length) {
    for(int i = 0; i < length; i++) {
        if(code1[i] != code2[i]) return false;
    }
    return true;
}