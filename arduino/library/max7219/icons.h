
// Heart Icon
byte icon_heart[] = {
  B00011000,
  B00111100,
  B01111110,
  B01111110,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

void Heart(int display, int x, int y, int speed) {   
  Draw(icon_heart, display, x, y, speed); 
}

// Smiley Face Icon
byte icon_smiley[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

void Smiley(int display, int x, int y, int speed) {   
  Draw(icon_smiley, display, x, y, speed); 
}

// Sad Face Icon
byte icon_sad[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
};

void Sad(int display, int x, int y, int speed) {   
  Draw(icon_sad, display, x, y, speed); 
}

// Checkmark Icon
byte icon_checkmark[] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B10010000,
  B01010000,
  B00110000,
  B00000000
};

void Checkmark(int display, int x, int y, int speed) {   
  Draw(icon_checkmark, display, x, y, speed); 
}

// Cross (X) Icon
byte icon_cross[] = {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};

void Cross(int display, int x, int y, int speed) {   
  Draw(icon_cross, display, x, y, speed); 
}

// Arrow Up Icon
byte icon_arrow_up[] = {
  B00011000,
  B00111100,
  B01111110,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00000000
};

void ArrowUp(int display, int x, int y, int speed) {   
  Draw(icon_arrow_up, display, x, y, speed); 
}

// Arrow Down Icon
byte icon_arrow_down[] = {
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

void ArrowDown(int display, int x, int y, int speed) {   
  Draw(icon_arrow_down, display, x, y, speed); 
}

// House Icon
byte icon_house[] = {
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B10000001,
  B10000001,
  B11111111,
  B00000000
};

void House(int display, int x, int y, int speed) {   
  Draw(icon_house, display, x, y, speed); 
}

// Bell Icon
byte icon_bell[] = {
  B00010000,
  B00111000,
  B00111000,
  B00111000,
  B01111100,
  B01111100,
  B00101000,
  B00010000
};

void Bell(int display, int x, int y, int speed) {   
  Draw(icon_bell, display, x, y, speed); 
}

// Wifi Signal Icon
byte icon_wifi[] = {
  B00000000,
  B00011000,
  B00100100,
  B01000010,
  B10000001,
  B00011000,
  B00011000,
  B00000000
};

void Wifi(int display, int x, int y, int speed) {   
  Draw(icon_wifi, display, x, y, speed); 
}

// Dragon Icon
byte icon_dragon[] = {
  B00011000,  //   **  
  B00111100,  //  **** 
  B01111110,  // ******  
  B11101111,  // *** ****
  B11111110,  // *******
  B01101100,  // ** **  
  B00101000,  //  * *   
  B00010000   //   *    
};

void Dragon(int display, int x, int y, int speed) {   
  Draw(icon_dragon, display, x, y, speed); 
}


