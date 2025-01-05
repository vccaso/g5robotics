void  A(int display, int x, int y, int speed) {  
  lc.setLed(display,x + 2,y+4,true);
  lc.setLed(display,x + 1,y+4,true);
  lc.setLed(display,x,y+3,true);
  lc.setLed(display,x + 3,y+3,true);
 
  lc.setLed(display,x + 1,y+2, true);
  lc.setLed(display,x + 2,y+2, true);
  lc.setLed(display,x,y+2, true);
  lc.setLed(display,x + 3,y+2, true);
  
  lc.setLed(display,x,y+1, true);
  lc.setLed(display,x,y, true);

  lc.setLed(display,x + 3,y+1, true);
  lc.setLed(display,x + 3,y, true);
  delay(speed);
}

void  B() {  
  lc.clearDisplay(0);

  delay(1000);
}

byte letter_B[] =
{
  B11111110, 
  B10000001,
  B10000010,
  B10000100,
  B10000100,
  B10000010,
  B10000100,
  B11111000
};

byte letter_C[] =
{
  B00000000,
  B00011000,
  B00100100,
  B01000010,
  B01000010,
  B00000000,
  B00000000,
  B00000000
};
byte letter_D[] =
{
  B00000000, 
  B01111110,
  B01000010,
  B00100100,
  B00011000,
  B00000000,
  B00000000,
  B00000000
};
byte letter_E[] =
{
  B11111111, 
  B11111111,
  B11000000,
  B11111111,
  B11111111,
  B11000000,
  B11111111,
  B11111111
};
byte letter_F[] =
{
  B11111111, 
  B11111111,
  B11000000,
  B11110000,
  B11110000,
  B11000000,
  B11000000,
  B11000000
};
byte letter_G[] =
{
  B00000000, 
  B00011000,
  B00100100,
  B01000010,
  B01010010,
  B00110100,
  B00000000,
  B00000000
};

byte letter_H[] =
{
  B00000000, // TODO
  B00011000,
  B00100100,
  B01000010,
  B01010010,
  B00110100,
  B00000000,
  B00000000
};

byte letter_I[] =
{
  B00000000, // TODO
  B01111110,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B01111110,
  B00000000
};

byte letter_J[] =
{
  B00011111, 
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B11111110,
  B00000000
};
byte letter_K[] =
{
  B11000001, 
  B11000001,
  B11000010,
  B11000100,
  B11111000,
  B11000100,
  B11000010,
  B11000001
};

void  Draw(byte arr[], int display, int x, int y, int speed) {  
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(display,i+y,arr[i]);
  }
  delay(speed);
}

void  C(int display, int x, int y, int speed) {  
  Draw(letter_C,display,x,y,speed);
}


void  D(int display, int x, int y, int speed) {  
  Draw(letter_D,display,x,y,speed);
}


void  E(int display, int x, int y, int speed) {  
  Draw(letter_E,display,x,y,speed);
}

void F_(int display, int x, int y, int speed) 
{  
  Draw(letter_F,display,x,y,speed);
}

void  G(int display, int x, int y, int speed) {  
  Draw(letter_G,display,x,y,speed);
}

void H(int display, int x, int y, int speed) {  
  Draw(letter_H,display,x,y,speed);
}

void  I(int display, int x, int y, int speed) {  
  Draw(letter_I,display,x,y,speed);
}

void  J(int display, int x, int y, int speed) {  
  Draw(letter_J,display,x,y,speed);
}

void  K(int display, int x, int y, int speed) {  
  Draw(letter_K,display,x,y,speed);
}

void  L() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  M() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  N() {  
  lc.clearDisplay(0);
lc.setLed(0,5,1, true);
lc.setLed(0,5,2, true);
lc.setLed(0,5,3, true);
lc.setLed(0,5,4, true);
lc.setLed(0,5,5, true);

lc.setLed(0,1,1, true);
lc.setLed(0,1,2, true);
lc.setLed(0,1,3, true);
lc.setLed(0,1,4, true);
lc.setLed(0,1,5, true);

lc.setLed(0,2,4, true);
lc.setLed(0,3,3, true);
lc.setLed(0,4,2, true);
  delay(1000);
}
void  O() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  P() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  Q() {  
  lc.clearDisplay(0);

  delay(1000);
}
void  R() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  S() {  
  lc.clearDisplay(0);

  delay(1000);
}


void T(int display) {
  lc.setLed(display,3,1,true);
  lc.setLed(display,3,2,true);
  lc.setLed(display,3,3,true);
  lc.setLed(display,3,4,true);
  lc.setLed(display,3,5,true);
  lc.setLed(display,2,5,true);
  lc.setLed(display,1,5,true);
  lc.setLed(display,4,5,true);
  lc.setLed(display,5,5,true);
  delay(300);
}

void  U() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  V() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  W() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  X() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  Y() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  Z() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  a() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  b() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  c() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  d() {  
  lc.clearDisplay(0);

  delay(1000);
}


void  e() {  
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

void f(int display, int x, int y, int speed) {  
  lc.clearDisplay(0);

  delay(1000);
}

void  g() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  h() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  i() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  j() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  k() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  l() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  m() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  n() {  
  lc.clearDisplay(0);

  delay(1000);
}
void  o() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  p() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  q() {  
  lc.clearDisplay(0);

  delay(1000);
}
void  r() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  s() {  
  lc.clearDisplay(0);

  delay(1000);
}


void t() {
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

void  u() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  v() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  w() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  x() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  y() {  
  lc.clearDisplay(0);

  delay(1000);
}

void  z() {  
  lc.clearDisplay(0);

  delay(1000);
}

