#define LED_BLUE D0 
#define LED_WHITE D5

#define LED_GREEN D6 
#define LED_YELLOW D7
#define LED_RED D8


void setup() {
  pinMode(LED_BLUE, OUTPUT);    // LED pin as output.
  pinMode(LED_WHITE, OUTPUT);    // LED pin as output.
  pinMode(LED_GREEN, OUTPUT);    // LED pin as output.
  pinMode(LED_YELLOW, OUTPUT);    // LED pin as output.
  pinMode(LED_RED, OUTPUT);    // LED pin as output.

  Serial.begin(9600);
  selfTest();
}

void loop() {
  Serial.println();
  delay(3000);
  selfTest();
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

  flickr(LED_BLUE);
  flickr(LED_BLUE);
  flickr(LED_BLUE);
  flickr(LED_WHITE);
  flickr(LED_WHITE);
  flickr(LED_WHITE);
  flickr(LED_RED);
  flickr(LED_RED);
  flickr(LED_RED);
  flickr(LED_YELLOW);
  flickr(LED_YELLOW);
  flickr(LED_YELLOW);
  flickr(LED_GREEN);
  flickr(LED_GREEN);
  flickr(LED_GREEN);
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
