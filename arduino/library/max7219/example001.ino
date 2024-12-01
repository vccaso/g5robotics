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


