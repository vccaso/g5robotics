// Number 0
byte number_0[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B10000001,  // *      * 
  B10000001,  // *      * 
  B10000001,  // *      * 
  B10000001,  // *      * 
  B01000010,  //  *    *  
  B00111100   //   ****   
};

// Number 1
byte number_1[] = {
  B00001000,  //     *    
  B00011000,  //    **    
  B00101000,  //   * *    
  B00001000,  //     *    
  B00001000,  //     *    
  B00001000,  //     *    
  B00001000,  //     *    
  B00111110   //   *****  
};

// Number 2
byte number_2[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B00000010,  //       *  
  B00000100,  //      *   
  B00001000,  //     *    
  B00010000,  //    *     
  B00100000,  //   *      
  B01111110   //  ******  
};

// Number 3
byte number_3[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B00000010,  //       *  
  B00011100,  //    ***   
  B00000010,  //       *  
  B00000010,  //       *  
  B01000010,  //  *    *  
  B00111100   //   ****   
};

// Number 4
byte number_4[] = {
  B00000100,  //      *   
  B00001100,  //     **   
  B00010100,  //    * *   
  B00100100,  //   *  *   
  B01000100,  //  *   *   
  B01111110,  //  ******  
  B00000100,  //      *   
  B00000100   //      *   
};

// Number 5
byte number_5[] = {
  B01111110,  //  ******  
  B01000000,  //  *       
  B01000000,  //  *       
  B01111100,  //  *****   
  B00000010,  //       *  
  B00000010,  //       *  
  B01000010,  //  *    *  
  B00111100   //   ****   
};

// Number 6
byte number_6[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B10000000,  // *        
  B10111100,  // * ****   
  B11000010,  // **    *  
  B10000010,  // *     *  
  B01000010,  //  *    *  
  B00111100   //   ****   
};

// Number 7
byte number_7[] = {
  B01111110,  //  ******  
  B00000010,  //       *  
  B00000100,  //      *   
  B00001000,  //     *    
  B00010000,  //    *     
  B00100000,  //   *      
  B01000000,  //  *       
  B10000000   // *        
};

// Number 8
byte number_8[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B01000010,  //  *    *  
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B01000010,  //  *    *  
  B01000010,  //  *    *  
  B00111100   //   ****   
};

// Number 9
byte number_9[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B01000010,  //  *    *  
  B00111110,  //   *****  
  B00000010,  //       *  
  B00000010,  //       *  
  B01000010,  //  *    *  
  B00111100   //   ****   
};


void Number0(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_0, display, x, y, speed);
}

void Number1(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_1, display, x, y, speed);
}

void Number2(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_2, display, x, y, speed);
}

void Number3(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_3, display, x, y, speed);
}

void Number4(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_4, display, x, y, speed);
}

void Number5(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_5, display, x, y, speed);
}

void Number6(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_6, display, x, y, speed);
}

void Number7(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_7, display, x, y, speed);
}

void Number8(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_8, display, x, y, speed);
}

void Number9(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(number_9, display, x, y, speed);
}
