// uLCD-144-G2 basic text demo program for uLCD-4GL LCD driver library

#include "mbed.h"
#include "uLCD_4DGL.h"
#define SAMPLE_RATE 512.0


DigitalIn BOTTOM_UP(D12);     // up bottom
DigitalIn BOTTOM_DOWN(D11);   // down bottom
DigitalIn BOTTOM_SELECT(D10); // select bottom
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(D7);
AnalogIn Ain(A0);
float ADCdata[512];
EventQueue queue_ADC_sampling(32 * EVENTS_EVENT_SIZE);
EventQueue queue_screen_print(32 * EVENTS_EVENT_SIZE);
Thread t_ADC_sampling;
Thread t_screen_print;

void freq_select();
void wave_generator();
void ADC_sampling();
void screen_print();

float d1 = 0;
float d2 = 0;
int temp_freq = 1;
float freq_state = 0;

int main()
{
    t_ADC_sampling.start(callback(&queue_ADC_sampling, &EventQueue::dispatch_forever));
    t_screen_print.start(callback(&queue_screen_print, &EventQueue::dispatch_forever));
    // initializing uLCD
    uLCD.cls();
    uLCD.text_width(3); //3X size text
    uLCD.text_height(3);
    // basic printf demo = 16 by 18 characters on screen
    uLCD.color(BLUE);
    uLCD.textbackground_color(WHITE);
    uLCD.locate(0,0);
    uLCD.printf("50Hz"); //blue text on black text
    uLCD.textbackground_color(GREEN);
    uLCD.locate(0,2);
    uLCD.printf("180Hz");
    uLCD.textbackground_color(WHITE);
    uLCD.locate(0,4);
    uLCD.printf("250Hz");
    aout = 0.0;
    while (1) {
      freq_select();
      //d1 = float(freq_state*6/10000);
      d1 = float(freq_state*0.9/0.4/10000);
      //d2 = float(freq_state*4/10000);
      d2 = float(freq_state*0.9/0.6/10000);
      wave_generator();
      queue_ADC_sampling.call(ADC_sampling);
      queue_screen_print.call(screen_print);

    }

} // end of main


void freq_select() {
  if (temp_freq == 0) {
    if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("50Hz");
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("180Hz");
      temp_freq = 1;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,0);
      uLCD.printf("50Hz");
      freq_state = 50;
    }
  }
  else if (temp_freq == 1) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,0);
      uLCD.printf("50Hz");
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,2);
      uLCD.printf("180Hz");
      temp_freq = 0;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,2);
      uLCD.printf("180Hz");
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,4);
      uLCD.printf("250Hz");
      temp_freq = 2;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("180Hz");
      freq_state = 180;
    }
  }
  else if (temp_freq == 2) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("180Hz");
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("250Hz");
      temp_freq = 1;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,4);
      uLCD.printf("250Hz");
      freq_state = 250;
    }
  }
}

void wave_generator() {
  aout = 0;
  wait_us(70);
  for (int i = 0; freq_state != 0 && i*d1 <= 0.9; i++) {
    aout = aout + d1;
    wait_us(70);
  }
  aout = 0.9;
  wait_us(70);
  for (int i = 0; freq_state != 0 && (0.9-i*d2) >= 0.0; i++) {
    aout = aout - d2;
    wait_us(70);
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

void ADC_sampling() {
  for (int i = 0; i < SAMPLE_RATE; i++){

    ADCdata[i] = Ain;

    ThisThread::sleep_for(1000ms/(int)SAMPLE_RATE);

  }

}

void screen_print() {
  for (int i = 0; i < SAMPLE_RATE; i++){

    printf("%f\r\n", ADCdata[i]);

    //ThisThread::sleep_for(100ms);

  }
}