// Plus (+) Symbol
byte symbol_plus[] = {
  B00011000,  //    **   
  B00011000,  //    **   
  B00011000,  //    **   
  B11111111,  // ********
  B11111111,  // ********
  B00011000,  //    **   
  B00011000,  //    **   
  B00011000   //    **   
};

// Minus (-) Symbol
byte symbol_minus[] = {
  B00000000,  //          
  B00000000,  //          
  B00000000,  //          
  B11111111,  // ********
  B11111111,  // ********
  B00000000,  //          
  B00000000,  //          
  B00000000   //          
};

// Multiply (×) Symbol
byte symbol_multiply[] = {
  B10000001,  // *      * 
  B01000010,  //  *    *  
  B00100100,  //   *  *   
  B00011000,  //    **    
  B00011000,  //    **    
  B00100100,  //   *  *   
  B01000010,  //  *    *  
  B10000001   // *      * 
};

// Division (÷) Symbol
byte symbol_division[] = {
  B00011000,  //    **    
  B00011000,  //    **    
  B00000000,  //          
  B11111111,  // ********
  B11111111,  // ********
  B00000000,  //          
  B00011000,  //    **    
  B00011000   //    **    
};

// Period (.) Symbol
byte symbol_period[] = {
  B00000000,  //          
  B00000000,  //          
  B00000000,  //          
  B00000000,  //          
  B00000000,  //          
  B00000000,  //          
  B00011000,  //    **    
  B00011000   //    **    
};

// Exclamation (!) Symbol
byte symbol_exclamation[] = {
  B00011000,  //    **    
  B00011000,  //    **    
  B00011000,  //    **    
  B00011000,  //    **    
  B00011000,  //    **    
  B00000000,  //          
  B00011000,  //    **    
  B00011000   //    **    
};

// Question (?) Symbol
byte symbol_question[] = {
  B00111100,  //   ****   
  B01000010,  //  *    *  
  B00000010,  //       *  
  B00011100,  //    ***   
  B00001000,  //     *    
  B00000000,  //          
  B00011000,  //    **    
  B00011000   //    **    
};

// Dollar ($) Symbol
byte symbol_dollar[] = {
  B00011000,  //    **    
  B00111110,  //   *****  
  B01000000,  //  *       
  B00111100,  //   ****   
  B00000010,  //       *  
  B01111100,  //  *****   
  B00011000,  //    **    
  B00000000   //          
};

// Ampersand (&) Symbol
byte symbol_ampersand[] = {
  B00111000,  //   ***    
  B01000100,  //  *   *   
  B01001000,  //  *  *    
  B00110000,  //   **     
  B01101010,  //  ** * *  
  B01000100,  //  *   *   
  B00111010,  //   *** *  
  B00000000   //          
};

// Percent (%) Symbol
byte symbol_percent[] = {
  B01100010,  //  **   *  
  B10100100,  // * *  *   
  B10101000,  // * * *    
  B01110000,  //  ***     
  B00011000,  //    **    
  B00101010,  //   * * *  
  B01001010,  //  *  * *  
  B10000110   // *    **  
};

// Equal (=) Symbol
byte symbol_equal[] = {
  B00000000,  //          
  B11111111,  // ********
  B00000000,  //          
  B00000000,  //          
  B00000000,  //          
  B11111111,  // ********
  B00000000,  //          
  B00000000   //          
};


void Plus(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_plus, display, x, y, speed);
}

void Minus(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_minus, display, x, y, speed);
}

void Multiply(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_multiply, display, x, y, speed);
}

void Division(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_division, display, x, y, speed);
}

void Period(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_period, display, x, y, speed);
}

void Exclamation(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_exclamation, display, x, y, speed);
}

void Question(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_question, display, x, y, speed);
}

void Dollar(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_dollar, display, x, y, speed);
}

void Ampersand(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_ampersand, display, x, y, speed);
}

void Percent(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_percent, display, x, y, speed);
}

void Equal(int display, int x, int y, int speed) {  
  lc.clearDisplay(display);
  Draw(symbol_equal, display, x, y, speed);
}
