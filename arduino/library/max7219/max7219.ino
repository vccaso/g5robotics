// MAX7219 - using Led Control library to display all   leds one by one
#include <LedControl.h>

int DIN = 8;
int CS = 9;
int CLK = 10;

LedControl lc = LedControl(DIN, CLK, CS, 8);

// Helper function to draw an 8x8 byte array on a specific display
void Draw(byte data[], int display, int x, int y, int speed) {
  for (int i = 0; i < 8; i++) {
    lc.setRow(display, i, data[i]);
  }
  delay(speed);
}

// Import libraries from the same folder
#include "faces.h"
#include "icons.h"
#include "letters.h"
#include "lowercase.h"
#include "numbers.h"
#include "symbols.h"

// Virtual buffer to track current state of all 8 displays
byte vBuf[8][8];

// Define a space character
byte char_space[] = {0, 0, 0, 0, 0, 0, 0, 0};

// Helper function to get data for a character
const byte *getCharData(char c) {
  if (c == ' ')
    return char_space;

  // Uppercase Letters
  if (c == 'A')
    return letter_A;
  if (c == 'B')
    return letter_B;
  if (c == 'C')
    return letter_C;
  if (c == 'D')
    return letter_D;
  if (c == 'E')
    return letter_E;
  if (c == 'F')
    return letter_F;
  if (c == 'G')
    return letter_G;
  if (c == 'H')
    return letter_H;
  if (c == 'I')
    return letter_I;
  if (c == 'J')
    return letter_J;
  if (c == 'K')
    return letter_K;
  if (c == 'L')
    return letter_L;
  if (c == 'M')
    return letter_M;
  if (c == 'N')
    return letter_N;
  if (c == 'O')
    return letter_O;
  if (c == 'P')
    return letter_P;
  if (c == 'Q')
    return letter_Q;
  if (c == 'R')
    return letter_R;
  if (c == 'S')
    return letter_S;
  if (c == 'T')
    return letter_T;
  if (c == 'U')
    return letter_U;
  if (c == 'V')
    return letter_V;
  if (c == 'W')
    return letter_W;
  if (c == 'X')
    return letter_X;
  if (c == 'Y')
    return letter_Y;
  if (c == 'Z')
    return letter_Z;

  // Lowercase Letters
  if (c == 'a')
    return letter_a;
  if (c == 'b')
    return letter_b;
  if (c == 'c')
    return letter_c;
  if (c == 'd')
    return letter_d;
  if (c == 'e')
    return letter_e;
  if (c == 'f')
    return letter_f;
  if (c == 'g')
    return letter_g;
  if (c == 'h')
    return letter_h;
  if (c == 'i')
    return letter_i;
  if (c == 'j')
    return letter_j;
  if (c == 'k')
    return letter_k;
  if (c == 'l')
    return letter_l;
  if (c == 'm')
    return letter_m;
  if (c == 'n')
    return letter_n;
  if (c == 'o')
    return letter_o;
  if (c == 'p')
    return letter_p;
  if (c == 'q')
    return letter_q;
  if (c == 'r')
    return letter_r;
  if (c == 's')
    return letter_s;
  if (c == 't')
    return letter_t;
  if (c == 'u')
    return letter_u;
  if (c == 'v')
    return letter_v;
  if (c == 'w')
    return letter_w;
  if (c == 'x')
    return letter_x;
  if (c == 'y')
    return letter_y;
  if (c == 'z')
    return letter_z;

  // Numbers
  if (c == '0')
    return number_0;
  if (c == '1')
    return number_1;
  if (c == '2')
    return number_2;
  if (c == '3')
    return number_3;
  if (c == '4')
    return number_4;
  if (c == '5')
    return number_5;
  if (c == '6')
    return number_6;
  if (c == '7')
    return number_7;
  if (c == '8')
    return number_8;
  if (c == '9')
    return number_9;

  // Symbols
  if (c == '+')
    return symbol_plus;
  if (c == '-')
    return symbol_minus;
  if (c == '*')
    return symbol_multiply;
  if (c == '/')
    return symbol_division;
  if (c == '.')
    return symbol_period;
  if (c == '!')
    return symbol_exclamation;
  if (c == '?')
    return symbol_question;
  if (c == '$')
    return symbol_dollar;
  if (c == '&')
    return symbol_ampersand;
  if (c == '%')
    return symbol_percent;
  if (c == '=')
    return symbol_equal;

  return char_space;
}

// Scrolling Directions
enum ScrollDir { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

// Helper function to update hardware from buffer
void updateDisplays() {
  for (int d = 0; d < 8; d++) {
    for (int r = 0; r < 8; r++) {
      lc.setRow(d, r, vBuf[d][r]);
    }
  }
}

// Function to scroll text across all 8 displays in 4 directions
void scrollText(String text, int speed, ScrollDir dir = DIR_LEFT) {
  if (dir == DIR_LEFT || dir == DIR_RIGHT) {
    // Add padding to ensure the text starts/ends off-screen
    if (dir == DIR_LEFT) text = text + "        ";
    else text = "        " + text;

    int len = text.length();
    // For LEFT, we go from index 0 to len-1. For RIGHT, we go from len-1 to 0.
    for (int i = 0; i < len; i++) {
      int charIdx = (dir == DIR_LEFT) ? i : (len - 1 - i);
      const byte *data = getCharData(text[charIdx]);

      for (int col = 0; col < 8; col++) {
        for (int r = 0; r < 8; r++) {
          // For LEFT, columns enter from the right (0 to 7).
          // For RIGHT, columns enter from the left (7 to 0).
          bool nextBit = (dir == DIR_LEFT) ? bitRead(data[r], 7 - col)
                                           : bitRead(data[r], col);

          if (dir == DIR_LEFT) {
            // Shift display 0 towards 7 (Leftwards movement)
            for (int d = 0; d <= 7; d++) {
              bool bitFallingOff = bitRead(vBuf[d][r], 7);
              vBuf[d][r] = (vBuf[d][r] << 1) | (nextBit ? 1 : 0);
              nextBit = bitFallingOff;
            }
          } else {
            // Shift display 7 towards 0 (Rightwards movement)
            for (int d = 7; d >= 0; d--) {
              bool bitFallingOff = bitRead(vBuf[d][r], 0);
              vBuf[d][r] = (vBuf[d][r] >> 1) | (nextBit ? 128 : 0);
              nextBit = bitFallingOff;
            }
          }
        }
        updateDisplays();
        delay(speed);
      }
    }
  } else {

    // Vertical scrolling logic (Paged)
    for (int i = 0; i < text.length(); i += 8) {
      String sub = text.substring(i, i + 8);
      while (sub.length() < 8)
        sub += " ";

      // Scroll in
      for (int step = 0; step < 8; step++) {
        for (int d = 0; d < 8; d++) {
          // Reverse character mapping: Display 0 is right, 7 is left
          const byte *data = getCharData(sub[7 - d]);
          if (dir == DIR_UP) {
            for (int r = 0; r < 7; r++)
              vBuf[d][r] = vBuf[d][r + 1];
            vBuf[d][7] = data[step];
          } else {
            for (int r = 7; r > 0; r--)
              vBuf[d][r] = vBuf[d][r - 1];
            vBuf[d][0] = data[7 - step];
          }
        }
        updateDisplays();
        delay(speed);
      }
      delay(speed * 5); // Pause to read

      // Scroll out
      for (int step = 0; step < 8; step++) {
        for (int d = 0; d < 8; d++) {
          if (dir == DIR_UP) {
            for (int r = 0; r < 7; r++)
              vBuf[d][r] = vBuf[d][r + 1];
            vBuf[d][7] = 0;
          } else {
            for (int r = 7; r > 0; r--)
              vBuf[d][r] = vBuf[d][r - 1];
            vBuf[d][0] = 0;
          }
        }
        updateDisplays();
        delay(speed);
      }
    }

  }
}

void setup() {
  for (int i = 0; i < 8; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 0);
    lc.clearDisplay(i);
    for (int r = 0; r < 8; r++)
      vBuf[i][r] = 0;
  }
}

void loop() {
  scrollText("Mom", 10, DIR_RIGHT);
  delay(500);
  scrollText("love you", 10, DIR_LEFT);
  delay(500);
  scrollText("Ethan", 10, DIR_UP);
  delay(500);
  scrollText("& Eli ", 10, DIR_DOWN);
  delay(500);
}
