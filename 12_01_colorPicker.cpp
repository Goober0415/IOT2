/*
 * Project ColorPicker
 * Author: Jamie Gavina
 * Date: 11/07/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "neopixel.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"

const int PIXELCOUNT = 2;
int color;
byte r, g, b,red,green,blue;
byte buf[6];
int len = EEPROM.length();
int addr = 0x00AE;
int val = 0x45;

void MQTT_connect();
bool MQTT_ping();
void pixelFill(int startP, int endP, int hex);
void colorChange(int color, byte *r, byte *g, byte *b);

/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

Adafruit_MQTT_Subscribe colors = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/colors");

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
  mqtt.subscribe(&colors);
  Serial.begin(9600);
  pixel.begin();
  pixel.setBrightness(80);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  MQTT_connect();
  MQTT_ping();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if(subscription == &colors){
      Serial.printf("Received from Adafruit: %s \n",(char *)colors.lastread );
      memcpy (buf, &colors.lastread[1],6); // strip off the ’#’
      Serial.printf("Buffer: %s \n",(char *)buf);
      color = strtol((char *) buf, NULL,16); // convert string to int (hex)
      Serial.printf("Buffer: 0x%02X \n", color);
      pixelFill(0, PIXELCOUNT, color);
      colorChange(color, &r, &g, &b);
      Serial.printf("R: 0x%02X | G: 0x%02X | B: 0x%02X", r, g, b);
      EEPROM.write(addr, r);
      EEPROM.write(addr+1, g);
      EEPROM.write(addr+2, b);
      red = EEPROM.read(addr);
      green = EEPROM.read(addr+1);
      blue = EEPROM.read(addr+2);
      Serial.printf("\nRed: 0x%02X | Green: 0x%02X | Blue: 0x%02X", red, green, blue);
      
    }
  }
}

void colorChange(int color, byte *r, byte *g, byte *b){
  *r = (color>>16)&0xFF;
  *g = (color>>8)&0xFF;
  *b = (color)&0xFF;
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}

void pixelFill(int startP, int endP, int hex){
  for(int i = startP; i <= endP; i++){
    pixel.setPixelColor(i, hex);
    pixel.show();
  }

    int8_t ret;
    
    if (mqtt.connected()) {
        return;
    }
    
    Serial.print("Connecting to MQTT... ");
    while ((ret = mqtt.connect()) != 0) {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);
    }
    Serial.println("MQTT Connected!");
}