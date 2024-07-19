// Kommunikation zum CO2-Sensor
#include <SoftwareSerial.h>
#include <Wire.h>              // Wire library (required for I2C devices)

// Kommunikation zum Luft-Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define LED_GREEN D6 
#define LED_YELLOW D7
#define LED_RED D8

// CO2 - Definitionen
SoftwareSerial co2Serial(D2, D1); // RX, TX Pinns festlegen
int co2Wert;              // CO2 Messwert in ppm

// Luft -Definitionen 
Adafruit_BME280 bme;
// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_I2C_ADDRESS  0x76

bool isInitialized = false;
bool bmeError = false;
 
void setup() {
  pinMode(LED_GREEN, OUTPUT);    // LED pin as output.
  pinMode(LED_YELLOW, OUTPUT);    // LED pin as output.
  pinMode(LED_RED, OUTPUT);    // LED pin as output.

  Serial.begin(9600);
  co2Serial.begin(9600);

  Wire.begin(D3,D4);
  if( bme.begin(BME280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println("bme280 Fehler !!");
    bmeError = true;
  }
}

void loop() {
  if(!isInitialized) {
    selfTest();
    isInitialized = true;
  }
  Serial.println();

  co2Wert = readCO2();
  setAmpel(co2Wert);

  readLuftWerte();
  delay(5000);
}

void readLuftWerte() {
  float temp = bme.readTemperature();  // get temperature in °C
  float humi = bme.readHumidity();     // get humidity in %
  float pres = bme.readPressure();     // get pressure in Pa
    Serial.print( "Temperatur ");
    Serial.print(temp);
    Serial.print("°C");
    Serial.println();
    Serial.print( "Luftfeutigkeit ");
    Serial.print(humi);
    Serial.print("%");
    Serial.println();
    Serial.print( "Luftdruck ");
    Serial.print(pres/100);
    Serial.print("hPa");
    Serial.println();
}

void selfTest() {
  greenOn();  
  delay(1000);
  yellowOn();
  delay(1000);
  redOn();
  delay(1000);
  allLedOff();
  delay(1000);
  greenBlink();
  yellowBlink();
  redBlink();
}

// Kommunikation mit MH-Z19 CO2 Sensor
int readCO2() {
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char answer[9];
  co2Serial.write(cmd, 9);
  co2Serial.readBytes(answer, 9);
  if (answer[0] != 0xFF) return -1;
  if (answer[1] != 0x86) return -1;
  int answerHigh = (int) answer[2]; // CO2 High Byte
  int answerLow = (int) answer[3];  // CO2 Low Byte
  int ppm = (256 * answerHigh) + answerLow;
  Serial.print("CO2 Gehalt:");
  Serial.print(ppm);
  Serial.println("ppm");

  return ppm;                         // Antwort des MH-Z19 CO2 Sensors in ppm
}

// Zimmerwetter
// grün 1 < 600
// grün 2 < 900
// gelb 1 < 1200
// gelb 2 < 1600
// rot 1 < 2000
// rot 2 > 2000
void setAmpel(int ppm) {
  allLedOff();
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

void allLedOff() {
  greenOff();
  yellowOff();
  redOff();
}
