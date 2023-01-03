//This is a modified code based on the graphictest_st7789.ino example
//from the Adafruit ST7735 and ST7789 Library.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_RST        15
#define TFT_CS         16
#define TFT_DC         17

#define V_LIM 280
#define H_LIM 240

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

char i;
int x, y;
char str[20];

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
  //use this initializer (uncomment) if using a 1.69" 280x240 TFT:
  tft.init(240, 280);           // Init ST7789 280x240

// Some ready-made 16-bit ('565') color settings:
// #define ST77XX_BLACK 0x0000
// #define ST77XX_WHITE 0xFFFF
// #define ST77XX_RED 0xF800
// #define ST77XX_GREEN 0x07E0
// #define ST77XX_BLUE 0x001F
// #define ST77XX_CYAN 0x07FF
// #define ST77XX_MAGENTA 0xF81F
// #define ST77XX_YELLOW 0xFFE0
// #define ST77XX_ORANGE 0xFC00

  tft.fillScreen(ST77XX_WHITE);
//  delay(2000);

// tft.fillScreen(ST77XX_WHITE);
// delay(10000);
// tft.fillScreen(ST77XX_RED);
// delay(10000);
// tft.fillScreen(ST77XX_GREEN);
// delay(10000);
// tft.fillScreen(ST77XX_BLUE);
// delay(10000);
// tft.fillScreen(ST77XX_CYAN);
// delay(10000);
// tft.fillScreen(ST77XX_MAGENTA);
// delay(10000);
// tft.fillScreen(ST77XX_YELLOW);
// delay(10000);
// tft.fillScreen(ST77XX_ORANGE);
// delay(10000);

//Draw a line from the origin of the screen (x=0,y=0)(corner near 1.69" label) 
//to the center of the screen (x=120, y=140)
//  tft.drawLine(120, 140, 0, 0, ST77XX_WHITE);
//  tft.drawLine(120, 140, 120, 0, ST77XX_RED);
//  tft.drawLine(120, 140, 240, 0, ST77XX_GREEN);
//  tft.drawLine(120, 140, 240, 140, ST77XX_BLUE);
//  tft.drawLine(120, 140, 240, 280, ST77XX_CYAN);
//  tft.drawLine(120, 140, 120, 280, ST77XX_MAGENTA);
//  tft.drawLine(120, 140, 0, 280, ST77XX_YELLOW);
//  tft.drawLine(120, 140, 0, 140, ST77XX_ORANGE);

// //Draw a circle at the center of the screen
// tft.drawCircle(120, 140, 1, ST77XX_WHITE);
// tft.drawCircle(120, 140, 10, ST77XX_RED);
// tft.drawCircle(120, 140, 20, ST77XX_GREEN);
// tft.drawCircle(120, 140, 30, ST77XX_CYAN);
// tft.drawCircle(120, 140, 40, ST77XX_MAGENTA);
// tft.drawCircle(120, 140, 50, ST77XX_YELLOW);
// tft.drawCircle(120, 140, 60, ST77XX_ORANGE);

  // tft.setTextSize(1);
  // tft.setTextColor(ST77XX_BLACK);
  // tft.setCursor(0, 100);
  // sprintf(str, "%d - %c", 85, 85);
  // tft.println(str);
}

void loop(){
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  x = 0;
  for(x = 0; x < H_LIM; x+=48){
    for(y = 0; y < V_LIM; y+=10){
      if(i < 255){
        tft.setCursor(x, y);
        sprintf(str, "%d - %c", i, i);
        tft.println(str);
        i++;
      }else{
        tft.fillRect(x, y, 48, 15, ST77XX_WHITE);
      }
    }
  }
  delay(5000);
  

  // tft.fillScreen(ST77XX_BLACK);
  // //to the center of the screen (x=120, y=140)
  // tft.drawLine(120, 140, 0, 0, ST77XX_WHITE);
  // delay(100);
  // tft.drawLine(120, 140, 120, 0, ST77XX_RED);
  // delay(100);
  // tft.drawLine(120, 140, 240, 0, ST77XX_GREEN);
  // delay(100);
  // tft.drawLine(120, 140, 240, 140, ST77XX_BLUE);
  // delay(100);
  // tft.drawLine(120, 140, 240, 280, ST77XX_CYAN);
  // delay(100);
  // tft.drawLine(120, 140, 120, 280, ST77XX_MAGENTA);
  // delay(100);
  // tft.drawLine(120, 140, 0, 280, ST77XX_YELLOW);
  // delay(100);
  // tft.drawLine(120, 140, 0, 140, ST77XX_ORANGE);
  // delay(100);

  // //Draw a circle at the center of the screen
  // tft.drawCircle(120, 140, 1, ST77XX_WHITE);
  // delay(100);
  // tft.drawCircle(120, 140, 10, ST77XX_RED);
  // delay(100);
  // tft.drawCircle(120, 140, 20, ST77XX_GREEN);
  // delay(100);
  // tft.drawCircle(120, 140, 30, ST77XX_CYAN);
  // delay(100);
  // tft.drawCircle(120, 140, 40, ST77XX_MAGENTA);
  // delay(100);
  // tft.drawCircle(120, 140, 50, ST77XX_YELLOW);
  // delay(100);
  // tft.drawCircle(120, 140, 60, ST77XX_ORANGE);
  // delay(100);
  // tft.fillScreen(ST77XX_BLACK);
  // tft.setTextWrap(false);
  // tft.fillScreen(ST77XX_BLACK);
  // tft.setCursor(0, 30);
  // tft.setTextColor(ST77XX_CYAN);
  // tft.setTextSize(4);
  // tft.println("Welcome");
  // tft.setTextColor(ST77XX_YELLOW);
  // tft.setTextSize(4);
  // tft.println("to aMDI");
  // tft.setTextColor(ST77XX_GREEN);
  // tft.setTextSize(4);
  // tft.println("Provost");
  // tft.setTextColor(ST77XX_MAGENTA);
  // tft.setTextSize(4);
  // tft.println("Mili!");
  // delay(10000);
}