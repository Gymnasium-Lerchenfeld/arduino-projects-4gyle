// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library
 
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
 
#include <Wire.h>
#include <DS3231.h>
 
// Which pin on the Arduino is connected to the NeoPixels?
//#define PIN    D9 // On Trinket or Gemma, suggest changing this to 1
 
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 7*13 // Popular NeoPixel ring size
 
// When setting up the NeoPixel library, we tell it how many digit,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel digit1(NUMPIXELS, 12, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digit2(NUMPIXELS, 11, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digit3(NUMPIXELS, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digit4(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(298, 8, NEO_GRB + NEO_KHZ800);
 
#define DELAYVAL 20 // Time (in milliseconds) to pause between digit
 
//SevenSegment segment;
 
// Helligkeit
int eingang=A0;
int input_2 = 3;   //Infrarotsensor 1
int input_3 = 2;   //Infrarotsensor 2
void handleIR();
void displayNumber(int n, Adafruit_NeoPixel &led, int r, int g, int b);
 
RTClib myRTC;
 
void displayTime(int, int, float);
void displaySecond(int, float);
 
void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif    
 
  // END of Trinket-specific code.
 
  digit1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  digit2.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  digit3.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  digit4.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.begin();
}
 
int redAmount[] = {0, 15, 15, 0, 0, 15, 15};
int greenAmount[] = {0, 15, 0, 15, 15, 15, 0};
int blueAmount[] = {15, 0, 0, 15, 0, 15, 15};
int color = 0;
int bordercolor = 0;
int arrayLength = 7;
long start;
long delta;
bool startet;
long start_2;
long delta_2;
bool startet_2;

void handleIR(){



    if(!startet) {
    if (!digitalRead(input_2)) {
      start = millis();
      startet = true;
      Serial.println("start1");
      
    }
  } else {
    if (digitalRead(input_2)) {
      startet = false;
      delta = millis() - start;
      if (delta < 1000){
        //Command nach kurzem Signal mit der Hand
        Serial.println("kurz1");
        color ++;
        if (color >= arrayLength){
            color = 0;
        }
        
      } else { 
      if (!digitalRead(input_2)) {
        delta = millis() - start;
        if (delta >= 1000){
          Serial.println("lang1");
          startet = false;
        }
        }
      }
    }
  }

  if(!startet_2) {  //2. Infrarotsensor
    if (!digitalRead(input_3)) {
      start_2 = millis();
      startet_2 = true;
    }
  } else {
    if (digitalRead(input_3)) {
      startet_2 = false;
      delta_2 = millis() - start_2;
      if (delta_2 < 1000){
        //Command für 2. kurzes Signal mit der Hand bei anderem Sensor
        Serial.println("kurz");
        bordercolor ++;
        if (bordercolor >= arrayLength){
            bordercolor = 0;
        }
      } else {
      if (!digitalRead(input_3)) {
        delta_2 = millis() - start_2;
        if (delta_2 >= 1000){
          //Command für sehr langes vorhalten der Hand
          startet_2 = false;
        }
      }
    }
  }
}
}

float getBrightness() {
  int sensorwert=analogRead(eingang);
  if(sensorwert <= 200) {
    return 0.2;
  } else if(sensorwert > 200 && sensorwert <= 400) {  
    return 0.4;
  } else if(sensorwert > 400 && sensorwert <= 600) {
      return 0.5;
  } else if(sensorwert >600 && sensorwert <= 800) {
      return 0.6;
  } else if(sensorwert >800 && sensorwert <= 950) {
      return 0.8;
  } else {
      return 1.0;
  }
}
 

void loop() {
  handleIR();
  digit1.clear(); // Set all pixel colors to 'off'
  digit2.clear(); // Set all pixel colors to 'off'
  digit3.clear(); // Set all pixel colors to 'off'
  digit4.clear(); // Set all pixel colors to 'off'
 
  float brightness = getBrightness();
  DateTime now = myRTC.now();
 
  displayTime(now.hour(), now.minute(), brightness);
  displaySecond(now.second(), brightness);
}
 
int lastMinute = 0;
int lastcolor = 0;
float oldBrightness = 0;
void displayTime(int hour, int minute, float brightness) {
  if(minute != lastMinute || lastcolor != color || oldBrightness != brightness) {
    lastcolor = color;
    oldBrightness = brightness;
    lastMinute = minute;
    int red = redAmount[color] * brightness;
    int green = greenAmount[color] * brightness;
    int blue = blueAmount[color] * brightness;
    displayNumber((int)hour/10, digit1, red, green, blue);
    displayNumber(hour%10, digit2, red, green, blue);
    displayNumber((int)minute/10, digit3, red, green, blue);
    displayNumber(minute%10, digit4, red, green, blue);
  }
}
 
void displayNumber(int n, Adafruit_NeoPixel &led, int r, int g, int b) {
  led.clear();
  switch (n) {
  case 0:
    for(int i=0;i<=38;i++) {
      led.setPixelColor(i, led.Color(r,g,b));
    }
    for(int i=52;i<=90;i++) {
      led.setPixelColor(i, led.Color(r,g,b));
    }
  break;
 
  case 1:
    for(int i=0;i<=12;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=52;i<=64;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
 
  case 2:
    for(int i=13;i<=77;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
 
  case 3:
    for(int i=0;i<=25;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=39;i<=77;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
 
  case 4:
    for(int i=0;i<=12;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=39;i<=64;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=78;i<=90;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
 
  case 5:
    for(int i=0;i<=25;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=39;i<=51;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=65;i<=90;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
 
  break;
 
  case 6:
    for(int i=0;i<=51;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=65;i<=90;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
 
  case 7:
    for(int i=0;i<=12;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=52;i<=77;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
 
  case 8:
    for(int i=0;i<=90;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
 
  break;
  
  case 9:
    for(int i=0;i<=25;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
    for(int i=39;i<=90;i++) {
      led.setPixelColor(i, led.Color(r, g, b));
    }
  break;
  }
  led.show();
}
 

int nLeds=5;
int offset=244;
void displaySecond(int s, float brightness) {
  int red = redAmount[bordercolor] * brightness;
  int green = greenAmount[bordercolor] * brightness;
  int blue = blueAmount[bordercolor] * brightness;
  digitalWrite(7, s%2 == 0?HIGH:LOW);
  if(s==0) {
    strip.clear();
  }
  for(int l=1;l<=nLeds;l++) {
      strip.setPixelColor((s*nLeds+l+offset)%298,strip.Color(red, green, blue));
  }
  strip.show();
}
 
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of digit minus one.
  /*
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // digit.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    digit1.setPixelColor(i, digit.Color(25, 0, 12));
    digit2.setPixelColor(i, digit.Color(r, g, b));
    digit3.setPixelColor(i, digit.Color(0, 0, b));
    digit4.setPixelColor(i, digit.Color(0, b, b));
 
    digit1.show();   // Send the updated pixel colors to the hardware.
    digit2.show();   // Send the updated pixel colors to the hardware.
    digit3.show();   // Send the updated pixel colors to the hardware.
    digit4.show();   // Send the updated pixel colors to the hardware.
 
    //delay(DELAYVAL); // Pause before next pass through loop
  }*/
 