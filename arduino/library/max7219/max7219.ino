//MAX7219 - using Led Control library to display all   leds one by one
#include <LedControl.h>

int DIN = 8;
int CS = 9;
int   CLK = 10;

LedControl lc=LedControl(DIN, CLK, CS,8);



void setup()   {

  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);
  lc.shutdown(4,false);
  lc.shutdown(5,false);
  lc.shutdown(6,false);
  lc.shutdown(7,false);
  lc.setIntensity(0,0);
  lc.setIntensity(1,0);
  lc.setIntensity(2,0);
  lc.setIntensity(3,0);
  lc.setIntensity(4,0);
  lc.setIntensity(5,0);
  lc.setIntensity(6,0);
  lc.setIntensity(7,0);
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
  lc.clearDisplay(4);
  lc.clearDisplay(5);
  lc.clearDisplay(6);
  lc.clearDisplay(7);
}

void   loop() {
  F_(0,0,0,100);
  // T();
  H(1,0,0,10);
  // A(0,2,1);
  // T(1);
  // A(2,0,0);
  // T(3);
  // A(4,1,1);
  // T(5);
  // x(6,0,0, 1000);
  // (0,0,0, 10);
  // (1,1,4, 10);
  // (2,0,0, 10);
  // (3,0,0, 1000);
  // T(7);
  // N();
  //happyFace();
  // for(int i=-6; i<8; i ++) {
  //     A(0,i,1,100);
  //     A(2,2,i, 100);
  //     lc.clearDisplay(0);
  //     lc.clearDisplay(2);
  // }
      
      
}


