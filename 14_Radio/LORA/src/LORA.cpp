/*
 * Project Lora
 * Author: Jamie Dowden-Duarte
 * Date: 11/25/25
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include <Adafruit_GPS.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "IoTTimer.h"

const int OLED_RESET = -1, OLEDADDR = 0x3C;
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_GPS GPS(&Wire);

// Define User and Credentials
String password = "AA4104132968BA2224299079021594AB"; // AES128 password
String myName = "Jamie";
const int RADIOADDRESS = 0xC3, TIMEZONE = -6;
const unsigned int UPDATE = 30000;

// Define Constants
const int RADIONETWORK = 7, SENDADDRESS = 302, D7LEDPIN = D7;

// Declare Variables
float lat, lon, alt;
int sat;
unsigned int lastGPS;

IoTTimer timer;

// Declare Functions
void getGPS(float *latitude, float *longitude, float *altitude, int *satellites);
void sendData(String name, float latitude, float longitude, int satelittes);
void reyaxSetup(String password);

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 5000);

  Serial1.begin(115200);
  reyaxSetup(password);

  display.begin(SSD1306_SWITCHCAPVCC, OLEDADDR);
  display.display();

  // Initialize GPS
  GPS.begin(0x10); // The I2C address to use is 0x10, can find this info using I2C Scanner
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPS.println(PMTK_Q_RELEASE);

  pinMode(D7LEDPIN, OUTPUT);
  timer.startTimer(10);
}

void loop()
{
  GPS.read();
  if (GPS.newNMEAreceived())
  {
    if (!GPS.parse(GPS.lastNMEA()))
    {
      return;
    }
  }

  if (millis() - lastGPS > UPDATE)
  {
    lastGPS = millis(); // reset the timer
    Serial.printf("\n=================================================================\n");
    Serial.printf("Lat: %0.6f, Lon: %0.6f, Alt: %0.6f, Satellites: %i\n", lat, lon, alt, sat);
    Serial.printf("=================================================================\n\n");
  }

  // listen for incoming lora messages and then send GPS back
  if (Serial1.available())
  {                                                // full incoming buffer: +RCV=203,50,35.08,9,-36,41
    String parse0 = Serial1.readStringUntil('=');  //+RCV
    String parse1 = Serial1.readStringUntil(',');  // address received from
    String parse2 = Serial1.readStringUntil(',');  // buffer length
    String parse3 = Serial1.readStringUntil(',');  // fuseSound
    String parse4 = Serial1.readStringUntil(',');  // fuseDust
    String parse5 = Serial1.readStringUntil(',');  // rssi
    String parse6 = Serial1.readStringUntil('\n'); // snr
    String parse7 = Serial1.readString();          // extra

    digitalWrite(D7LEDPIN, HIGH);
    timer.startTimer(5000);

    Serial.printf("parse0: %s\nparse1: %s\nparse2: %s\nparse3: %s\nparse4: %s\nparse5: %s\nparse6: %s\nparse7: %s\n", parse0.c_str(), parse1.c_str(), parse2.c_str(), parse3.c_str(), parse4.c_str(), parse5.c_str(), parse6.c_str(), parse7.c_str());
    // display.clearDisplay();
    // display.setTextSize(1);
    // display.setTextColor(WHITE);
    // display.setCursor(0,0);
    // display.clearDisplay();
    // display.printf("Sound: %s\nDisplay: %s",parse3, parse4);
    // display.display();
    // delay(100);

    getGPS(&lat, &lon, &alt, &sat);

    sendData(myName, lat, lon, sat);
  }

  else
  {
    digitalWrite(D7LEDPIN, LOW);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.printf("Lat: %0.6f\nLon: %0.6f\nAlt: %0.6f\nSatellites: %i", lat, lon, alt, sat);
  display.display();

  if (timer.isTimerReady())
  {
    digitalWrite(D7LEDPIN, LOW);
  }
}

void getGPS(float *latitude, float *longitude, float *altitude, int *satellites)
{
  int theHour;

  theHour = GPS.hour + TIMEZONE;
  if (theHour < 0)
  {
    theHour = theHour + 24;
  }

  Serial.printf("Time: %02i:%02i:%02i:%03i\n", theHour, GPS.minute, GPS.seconds, GPS.milliseconds);
  Serial.printf("Dates: %02i-%02i-%02i\n", GPS.month, GPS.day, GPS.year);
  Serial.printf("Fix: %i, Quality: %i", (int)GPS.fix, (int)GPS.fixquality);
  if (GPS.fix)
  {
    *latitude = GPS.latitudeDegrees;
    *longitude = GPS.longitudeDegrees;
    *altitude = GPS.altitude;
    *satellites = (int)GPS.satellites;
  }
}

void sendData(String name, float latitude, float longitude, int satelittes)
{
  char buffer[60];
  sprintf(buffer, "AT+SEND=%i,60,%f,%f,%i,%s\r\n", SENDADDRESS, latitude, longitude, satelittes, name.c_str());
  Serial1.printf("%s", buffer);
  // Serial1.println(buffer);
  delay(1000);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply from send\n");
    String reply = Serial1.readStringUntil('\n');
    Serial.printf("Send reply: %s\n", reply.c_str());
  }
}

void reyaxSetup(String password)
{
  // following four paramaters have most significant effect on range
  // recommended within 3 km: 10,7,1,7
  // recommended more than 3 km: 12,4,1,7
  const int SPREADINGFACTOR = 10;
  const int BANDWIDTH = 7;
  const int CODINGRATE = 1;
  const int PREAMBLE = 7;
  String reply; // string to store replies from module

  Serial1.printf("AT+ADDRESS=%i\r\n", RADIOADDRESS); // set the radio address
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply from address\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply address: %s\n", reply.c_str());
  }

  Serial1.printf("AT+NETWORKID=%i\r\n", RADIONETWORK); // set the radio network
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply from networkid\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply network: %s\n", reply.c_str());
  }

  Serial1.printf("AT+CPIN=%s\r\n", password.c_str());
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply from password\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Reply: %s\n", reply.c_str());
  }

  Serial1.printf("AT+PARAMETER=%i,%i,%i,%i\r\n", SPREADINGFACTOR, BANDWIDTH, CODINGRATE, PREAMBLE);
  delay(200);
  if (Serial1.available() > 0)
  {
    reply = Serial1.readStringUntil('\n');
    Serial.printf("reply: %s\n", reply.c_str());
  }

  Serial1.printf("AT+ADDRESS?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Address: %s\n", reply.c_str());
  }

  Serial1.printf("AT+NETWORKID?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Network: %s\n", reply.c_str());
  }

  Serial1.printf("AT+PARAMETER?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("RadioParameters: %s\n", reply.c_str());
  }

  Serial1.printf("AT+CPIN?\r\n");
  delay(200);
  if (Serial1.available() > 0)
  {
    Serial.printf("Awaiting Reply\n");
    reply = Serial1.readStringUntil('\n');
    Serial.printf("Radio Password: %s\n", reply.c_str());
  }
}