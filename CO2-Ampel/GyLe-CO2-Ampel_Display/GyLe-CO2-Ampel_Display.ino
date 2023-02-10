//EINBINDUNG VON LIBRARIES

// Kommunikation zum CO2-Sensor
#include <SoftwareSerial.h>
#include <Wire.h>              // Wire library (required for I2C devices)

// Kommunikation zum OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Kommunikation zum Luft-Sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>



//VARIABLENDEKLARATION

#define LED_BLUE D0
#define LED_WHITE D5
#define LED_GREEN D6
#define LED_YELLOW D7
#define LED_RED D8
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// CO2-Definitionen
SoftwareSerial co2Serial(D2, D1); // RX, TX Pinns festlegen
int co2Wert = -1;              // CO2 Messwert in ppm

// Luft-Definitionen
Adafruit_BME280 bme;

// define device I2C address: 0x76 or 0x77 (0x77 is library default address)
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME280_I2C_ADDRESS  0x76
float temp = 0.0;
float humi = 0.0;
float pres = 0.0;

bool bmeError = false;


//Definition der Pin-Modi  (Ausgabe 1- oder 0-Wert)
void setup() {
  pinMode(LED_BLUE, OUTPUT);    // LED pin as output.
  pinMode(LED_WHITE, OUTPUT);    // LED pin as output.
  pinMode(LED_GREEN, OUTPUT);    // LED pin as output.
  pinMode(LED_YELLOW, OUTPUT);    // LED pin as output.
  pinMode(LED_RED, OUTPUT);    // LED pin as output.

  Serial.begin(9600);
  co2Serial.begin(9600);
  Wire.begin(D3, D4);
  if ( bme.begin(BME280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    Serial.println(F("bme280 Fehler !!"));
    bmeError = true;
    for (;;); // Don't proceed, loop forever
  }
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  //Clear the buffer
  display.clearDisplay();


  //Aufruf der Methode selfTest() -weiter unten im Code
  selfTest();
}

void loop() {
  Serial.println();

  int cnt = 0;
  co2Wert = -1;
  while (cnt < 5 && co2Wert < 0 ) {
    co2Wert = readCO2();
    cnt ++;
  }
  if (co2Wert < 0 ) {
    redBlink();
    redBlink();
    redBlink();
  } else {
    setCO2Ampel(co2Wert);
  }
  readLuftWerte();
  setHumidityAmpel();

  //Aufruf der Methode displayValue() - weiter unten im Code
  displayValue();    // Draw text


  // Wert in Millisekunden
  // 1 Minute = 60000
  // 3 Minuten = 180000
  // delay(60000);
  delay(20000);

}
void displayValue(void) {

  display.clearDisplay();
  display.setTextSize(1); // Draw 1X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextWrap(false);
  display.print(F("Temperature: "));
  display.print(temp);
  display.println("^C");
  display.println();
  display.print(F("Huimidity: "));
  display.print(humi);
  display.println("%");
  display.println();
  display.print(F("Pressure: "));
  display.print(pres);
  display.println("hPa");
  display.println();
  display.print(F("CO2: "));
  display.print(co2Wert);
  display.println("ppm");

  display.display();      // Show initial text
  //delay(100);

  // Scroll in various directions, pausing in-between:
  //display.startscrollleft(0x00, 0x0F);
  //delay(2000);

}
void readLuftWerte() {
  temp = bme.readTemperature();  // get temperature in °C
  humi = bme.readHumidity();     // get humidity in %
  pres = bme.readPressure() / 100;   // get pressure in Pa
  Serial.print("Temperatur ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Luftfeuchtigkeit ");
  Serial.print(humi);
  Serial.println("%");
  Serial.print("Luftdruck ");
  Serial.print(pres);
  Serial.println("hPa");

}

void selfTest() {
  display.clearDisplay();
  display.setTextSize(2); // Draw 1X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);

  //display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Blue"));
  display.display();
  delay(500);
  blueOn();
  delay(1000);

  display.setCursor(0, 48);
  //display.clearDisplay();
  display.println(F("White"));
  display.display();
  delay(500);
  whiteOn();
  delay(1000);

  display.setCursor(60, 48);
  //display.clearDisplay();
  display.println(F("Green"));
  display.display();
  delay(500);
  greenOn();
  delay(1000);

  //display.clearDisplay();
  display.setCursor(60, 24);
  display.println(F("Yellow"));
  display.display();
  delay(500);
  yellowOn();
  delay(1000);

  //display.clearDisplay();
  display.setCursor(60, 0);
  display.println(F("Red"));
  display.display();
  delay(500);
  redOn();
  delay(1000);

  display.clearDisplay();
  display.setCursor(0, 32);
  display.print(F("All LED Off")), allLedOff();
  display.display();
  display.clearDisplay();
  allLedOff();
  delay(2000);

  display.clearDisplay();
  display.setCursor(0, 24);
  display.println(F("Blue"));
  display.print(F("blink"));
  display.display();
  delay(500);
  blueBlink();
  delay(1000);

  display.clearDisplay();
  display.setCursor(0, 24);
  display.println(F("White"));
  display.print(F("blink"));
  display.display();
  delay(500);
  whiteBlink();
  delay(1000);

  display.clearDisplay();
  display.setCursor(0, 24);
  display.println(F("Green"));
  display.print(F("blink"));
  display.display();
  delay(500);
  greenBlink();
  delay(1000);

  display.clearDisplay();
  display.setCursor(0, 24);
  display.println(F("Yellow"));
  display.print(F("blink"));
  display.display();
  delay(500);
  yellowBlink();
  delay(1000);

  display.clearDisplay();
  display.setCursor(0, 24);
  display.println(F("Red"));
  display.print(F("blink"));
  display.display();
  delay(500);
  redBlink();
  delay(1000);
  
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.println("LED-Test");
  display.print("completed");
  display.display(); 
  delay(3000);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 24);
  display.print("LEDs ok?");
  display.display();
  delay(3000);
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("If Not"));
  display.println(F("Contact"));
  display.print(F("Support"));
  display.display();
  delay(3000);
  display.clearDisplay();
  
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

// unter 30% weiss
// 30% < humi < 60% weiss + blau
// > 60% blau
void setHumidityAmpel() {
  if (humi < 40) {
    whiteOn();
  } else if (humi < 60) {
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
  if (ppm < 600) {
    greenOn();
  } else if (ppm < 900) {
    greenOn();
    yellowOn();
  } else if (ppm < 1200) {
    yellowOn();
  } else if (ppm < 1600) {
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
