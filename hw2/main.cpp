// uLCD-144-G2 basic text demo program for uLCD-4GL LCD driver library

#include "mbed.h"
#include "uLCD_4DGL.h"


DigitalIn BOTTOM_UP(D12);     // up bottom
DigitalIn BOTTOM_DOWN(D11);   // down bottom
DigitalIn BOTTOM_SELECT(D10); // select bottom
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(D7);

void freq_select();
void wave_generator();

float d1 = float(720.0f*6/10000);
float d2 = float(720.0f*4/10000);
int temp_freq = 1;
float freq_state = 720;

int main()
{
    // initializing uLCD
    uLCD.cls();
    uLCD.text_width(3); //3X size text
    uLCD.text_height(3);
    // basic printf demo = 16 by 18 characters on screen
    uLCD.color(BLUE);
    uLCD.textbackground_color(WHITE);
    uLCD.locate(0,0);
    uLCD.printf("500Hz"); //blue text on black text
    uLCD.textbackground_color(GREEN);
    uLCD.locate(0,2);
    uLCD.printf("720Hz");
    uLCD.textbackground_color(WHITE);
    uLCD.locate(0,4);
    uLCD.printf("900Hz");
    aout = 0.0;
    while (1) {
      //freq_select();
      wave_generator();
    }

} // end of main


void freq_select() {
  if (temp_freq == 0) {
    if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("500Hz");
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("720Hz");
      temp_freq = 1;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,0);
      uLCD.printf("500Hz");
      freq_state = 0;
    }
  }
  else if (temp_freq == 1) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,0);
      uLCD.printf("500Hz");
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,2);
      uLCD.printf("720Hz");
      temp_freq = 0;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,2);
      uLCD.printf("720Hz");
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,4);
      uLCD.printf("900Hz");
      temp_freq = 2;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("720Hz");
      freq_state = 1;
    }
  }
  else if (temp_freq == 2) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("720Hz");
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("900Hz");
      temp_freq = 1;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,4);
      uLCD.printf("900Hz");
      freq_state = 2;
    }
  }
}

void wave_generator() {
  aout = 0;
  wait_us(100);
  for (int i = 0; i*d1 <= 0.9; i++) {
    aout = aout + d1;
    wait_us(100);
  }
  aout = 0.9;
  wait_us(100);
  for (int i = 0; (0.9-i*d2) >= 0.0; i++) {
    aout = aout - d2;
    wait_us(100);
  }
  /*
  aout = 0;
  wait_us(15);
  for (int i = 0; i < 15; i++) {
    aout = aout + 0.9/16;
    wait_us(15);
  }
  aout = 0.9;
  wait_us(15);
  for (int i = 0; i < 23; i++) {
    aout = aout - 0.9/24;
    wait_us(15);
  }
  */
  /*
  while (aout <= 0.9) {
    aout = aout + d1;
    printf("%f\n", (float)aout);
    wait_us(100);
  }
  while (aout >= 0.0) {
    aout = aout - d2;
    printf("%f\n", (float)aout);
    wait_us(100);
  }
  */
}