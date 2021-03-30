#include "mbed.h"
#include "uLCD_4DGL.h"
#define SAMPLE_RATE 200.0

DigitalIn BOTTOM_UP(D12);     // up bottom
DigitalIn BOTTOM_DOWN(D11);   // down bottom
DigitalIn BOTTOM_SELECT(D10); // select bottom
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(D7);
AnalogIn Ain(A0);

float ADCdata[270];
EventQueue queue_ADC_sampling(32 * EVENTS_EVENT_SIZE);
EventQueue queue_screen_print(32 * EVENTS_EVENT_SIZE);
Thread t_ADC_sampling;
Thread t_screen_print;
float d1 = 0;
float d2 = 0;
int temp_freq = 4;
float freq_state = 0;
const float FREQUENCY[10] = {3, 5, 10, 20, 37, 50, 60, 70, 85, 100};

void freq_select();
void wave_generator();
void ADC_sampling();
void screen_print();

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
    uLCD.printf("%dHz", (int)FREQUENCY[3]); //blue text on black text
    uLCD.textbackground_color(GREEN);
    uLCD.locate(0,2);
    uLCD.printf("%dHz", (int)FREQUENCY[4]);
    uLCD.textbackground_color(WHITE);
    uLCD.locate(0,4);
    uLCD.printf("%dHz", (int)FREQUENCY[5]);
    
    aout = 0.0;
    
    queue_ADC_sampling.call(ADC_sampling);

    while (1) {
      freq_select();
      d1 = float(freq_state*0.9/0.4/10000);
      d2 = float(freq_state*0.9/0.6/10000);
      wave_generator();
    } // end of while loop

} // end of main

// selecting frequency according to the bottom
void freq_select() {
  if (temp_freq == 0) {
    if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 1) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 2) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(BLACK);
      uLCD.printf(" ");
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 3) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(BLACK);
      uLCD.printf(" ");
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 4) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 5) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 6) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 7) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+2]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 8) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,0);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-2]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(BLACK);
      uLCD.printf(" ");
      temp_freq--;
    }
    else if (BOTTOM_DOWN == 1) {
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq+1]);
      temp_freq++;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
  else if (temp_freq == 9) {
    if (BOTTOM_UP == 1) {
      uLCD.textbackground_color(GREEN);
      uLCD.locate(0,2);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq-1]);
      uLCD.textbackground_color(WHITE);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      temp_freq--;
    }
    if (BOTTOM_SELECT == 1) {
      uLCD.textbackground_color(RED);
      uLCD.locate(0,4);
      uLCD.printf("%dHz", (int)FREQUENCY[temp_freq]);
      freq_state = FREQUENCY[temp_freq];
    }
  }
}

// generating wave according to the frequency selected
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
}

// sampling the wave generated with the sampling rate 'SAMPLE_RATE' per second
// and save the data into the array 'ADCdata'
void ADC_sampling() {

  ThisThread::sleep_for(3000ms);

  for (int i = 0; i < SAMPLE_RATE; i++){

    ADCdata[i] = Ain;

    ThisThread::sleep_for(1000ms/(int)SAMPLE_RATE);

  }
  queue_screen_print.call(screen_print);
}

// print out the data which is saved in the array 'ADCdata'
void screen_print() {
  for (int i = 0; i < SAMPLE_RATE; i++){

    printf("%f\r\n", ADCdata[i]);
    
    ThisThread::sleep_for(10ms);

  }
  queue_ADC_sampling.call(ADC_sampling);
}