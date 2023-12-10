/**
 *  Brief:
 *  This program defines a CO2-Ampel
 *  It uses the CO2-Sensor MH-Z19 and the BME280 Sensor
 *  The CO2-Ampel has 5 LEDs
 * - white: humidity < 30%
 * - blue: humidity < 60%
 * - green: CO2 < 600 ppm
 * - yellow: CO2 < 1200 ppm
 * - red: CO2 > 2000 ppm
 *  
 */
#include "CO2-Ampel.h"

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SoftwareSerial.h>

// Kommunikation zum Luft-Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// CO2 - Definitionen
SoftwareSerial co2Serial(D2, D1); // RX, TX Pinns festlegen

// Luft -Definitionen 
Adafruit_BME280 bme;
// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_I2C_ADDRESS  0x76

bool bmeError = false;

void hardware_setup() {
  pinMode(LED_BLUE, OUTPUT);    // LED pin as output.
  pinMode(LED_WHITE, OUTPUT);    // LED pin as output.
  pinMode(LED_GREEN, OUTPUT);    // LED pin as output.
  pinMode(LED_YELLOW, OUTPUT);    // LED pin as output.
  pinMode(LED_RED, OUTPUT);    // LED pin as output.

  Serial.println();
  
  co2Serial.begin(9600);
  Wire.begin(D3,D4);
  if( bme.begin(BME280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println("bme280 Fehler !!");
    bmeError = true;
  }
  selfTest();   
}
void hardware_loop() {
  int cnt = 0;
  co2Wert=-1;
  while(cnt < 10 && co2Wert < 0 ) {
    Serial.println("Reading CO2 value:...." + co2Wert);
    co2Wert = readCO2();
    cnt ++;
  }
  if(co2Wert < 0 ) {
    redBlink();
    redBlink();
    redBlink();
  } else {
    setCO2Ampel(co2Wert);
  }
  readLuftWerte();
  setHumidityAmpel();
}

/*******************************************************************/
void readLuftWerte() {
  temp = bme.readTemperature();  // get temperature in °C
  humi = bme.readHumidity();     // get humidity in %
  pres = bme.readPressure()/100;     // get pressure in Pa
    Serial.print( "Temperatur ");
    Serial.print(temp);
    Serial.print("°C");
    Serial.println();
    Serial.print( "Luftfeutigkeit ");
    Serial.print(humi);
    Serial.print("%");
    Serial.println();
    Serial.print( "Luftdruck ");
    Serial.print(pres);
    Serial.print("hPa");
    Serial.println();
}

void selfTest() {
  blueOn();  
  delay(1000);
  whiteOn();  
  delay(1000);
  greenOn();  
  delay(1000);
  yellowOn();
  delay(1000);
  redOn();
  delay(1000);
  allLedOff();
  delay(1000);
  blueBlink();
  whiteBlink();
  greenBlink();
  yellowBlink();
  redBlink();
}

// Kommunikation mit MH-Z19 CO2 Sensor
int readCO2() {
  Serial.print("CO2 Gehalt:");
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char answer[9];
  co2Serial.write(cmd, 9);
  co2Serial.readBytes(answer, 9);
  if (answer[0] != 0xFF) { 
    Serial.println(" keine Daten erhalten");
    return -1;
  }
  if (answer[1] != 0x86) {
     Serial.println(" keine Daten erhalten");
    return -1;
  }
  int answerHigh = (int) answer[2]; // CO2 High Byte
  int answerLow = (int) answer[3];  // CO2 Low Byte
  int ppm = (256 * answerHigh) + answerLow;
  Serial.print(ppm);
  Serial.println("ppm");

  return ppm;                         // Antwort des MH-Z19 CO2 Sensors in ppm
}

// unter 30% weiss
// 30% < humi < 60% weiss + blau
// > 60% blau
void setHumidityAmpel() {
  whiteOff();
  blueOff();
  if(humi < 40) {
    whiteOn();
  } else if(humi < 60) {
    whiteOn();
    blueOn();
  } else {
    blueOn();
  }
}

// Zimmerwetter
// grün 1 < 600
// grün 2 < 900
// gelb 1 < 1200
// gelb 2 < 1600
// rot 1 < 2000
// rot 2 > 2000
void setCO2Ampel(int ppm) {
    greenOff();
    yellowOff();
    redOff(); 
  if(ppm < 600) {
    greenOn();
  } else if(ppm < 900) {
    greenOn();
    yellowOn(); 
  } else if(ppm < 1200) {
    yellowOn();     
  } else if(ppm < 1600) {
    yellowOn();
    redOn();     
  } else {
    redBlink();
    redBlink();
    redBlink();
    redOn();
  }
}

void blueBlink() {
  blink(LED_BLUE);
}
void whiteBlink() {
  blink(LED_WHITE);
}
void greenBlink() {
  blink(LED_GREEN);
}
void yellowBlink() {
  blink(LED_YELLOW);
}
void redBlink() {
  blink(LED_RED);
}

void blink(int pin) {
  digitalWrite(pin, HIGH);
  delay(1000);
  digitalWrite(pin, LOW);  
  delay(500);
}

void flickr(int pin) {
  digitalWrite(pin, HIGH);
  delay(300);
  digitalWrite(pin, LOW);  
  delay(600);
}

void whiteOn() {
  digitalWrite(LED_WHITE, HIGH);
}
void whiteOff() {
  digitalWrite(LED_WHITE, LOW);
}
void blueOn() {
  digitalWrite(LED_BLUE, HIGH);
}
void blueOff() {
  digitalWrite(LED_BLUE, LOW);
}
void greenOn() {
  digitalWrite(LED_GREEN, HIGH);
}
void greenOff() {
  digitalWrite(LED_GREEN, LOW);
}
void yellowOn() {
  digitalWrite(LED_YELLOW, HIGH);
}
void yellowOff() {
  digitalWrite(LED_YELLOW, LOW);
}
void redOn() {
  digitalWrite(LED_RED, HIGH);
}
void redOff() {
  digitalWrite(LED_RED, LOW);
}

void allLedOn() {
  blueOn();
  whiteOn();
  greenOn();
  yellowOn();
  redOn();
}
void allLedOff() {
  whiteOff();
  blueOff();
  greenOff();
  yellowOff();
  redOff();
}
