#ifndef MY_AMPEL_H
  #define MY_AMPEL_H
  
  #define LED_BLUE D0 
  #define LED_WHITE D5
  
  #define LED_GREEN D6 
  #define LED_YELLOW D7
  #define LED_RED D8
  
  int co2Wert;              // CO2 Messwert in ppm
  float temp;
  float humi;
  float pres;
#endif

void hardware_setup();
void hardware_loop();