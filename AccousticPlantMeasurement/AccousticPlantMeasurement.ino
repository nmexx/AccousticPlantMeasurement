#include <Wire.h>
#include "DS3231.h"
#include <SPI.h>
#include <SD.h>


#define analogInPin A2
#define Timer0_Out 6 // 8bit
#define Clockspeed 16000000
#define Prescaler_Timer0 8
const int chipSelect = 10;

RTClib RTC;
int timer = 0;
int sensorValue = 0;
String dataString = "";

void setup() {
  SD.begin(chipSelect);
  pinMode(Timer0_Out, OUTPUT);
  pinMode(analogInPin, INPUT);
  Serial.begin(9600);
  while (!Serial) {
  }

  TCCR0A = _BV(WGM01)| _BV(WGM00)| _BV(COM0A0)| _BV(COM0B0);  
  TCCR0B = _BV(CS00)| _BV(WGM02);
  OCR0A = 205;
cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 0.25 Hz increments
OCR1A = 62499; // = 16000000 / (1024 * 0.25) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 1024 prescaler
TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
// enable timer compare interrupt
TIMSK1 |= (1 << OCIE1A);
sei(); // allow interruptsinterrupts


  

}

void loop() {
  if(timer ==1)
  {
   dataString = "";
   sensorValue = analogRead(analogInPin);
    DateTime now = RTC.now();
    dataString += now.year();
    dataString += '/';
    dataString += now.month();
     dataString += '/';
   dataString += now.day();
    dataString += ' ';
dataString += now.hour();
dataString += ':';
dataString += now.minute();
dataString += ':';
dataString += now.second();
dataString += "\t";
dataString += sensorValue;
   File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
   timer=0;
  }
}

ISR(TIMER1_COMPA_vect){
  timer = !timer; 
}
