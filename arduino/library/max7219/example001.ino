//MAX7219 - using Led Control library to display all   leds one by one
#include <LedControl.h>

int DIN = 12;
int CS = 10;
int   CLK = 11;

LedControl lc=LedControl(DIN, CLK, CS,0);



void setup()   {

  lc.shutdown(0,false);
  lc.setIntensity(0,0);
  lc.clearDisplay(0);
}

void   loop() {
  E();
  T();
}

void  E() {  
  lc.clearDisplay(0);
  lc.setLed(0,1,1,true);
  lc.setLed(0,1,2,true);
  lc.setLed(0,1,3,true);
  lc.setLed(0,1,4,true);
  lc.setLed(0,1,5,true);
  lc.setLed(0,2,1,true);
  lc.setLed(0,3,1,true);
  lc.setLed(0,2,3,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,2,5,true);
  lc.setLed(0,3,5,true);
  delay(1000);
}

void T() {
  lc.clearDisplay(0);
  lc.setLed(0,3,1,true);
  lc.setLed(0,3,2,true);
  lc.setLed(0,3,3,true);
  lc.setLed(0,3,4,true);
  lc.setLed(0,3,5,true);
  lc.setLed(0,2,5,true);
  lc.setLed(0,1,5,true);
  lc.setLed(0,4,5,true);
  lc.setLed(0,5,5,true);
  delay(1000);
}

