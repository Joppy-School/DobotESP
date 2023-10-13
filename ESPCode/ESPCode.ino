#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);


const int redpin = 13;
const int yellowpin = 23;
const int greenpin = 4;

const int ConstantGND = 34;
const int BusyPin = 35;
const int BlueSignal = 16;
const int GreenSignal = 17;
const int RedSignal = 18;

const int stopPin = 25;
const int Stopping = 26;

const int trackerpin = 32;

void setup(void) {
  Serial.begin(19200);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  pinMode(ConstantGND, INPUT);
  pinMode(BusyPin, INPUT);

  pinMode(trackerpin, INPUT);

  pinMode(BlueSignal, OUTPUT);
  pinMode(GreenSignal, OUTPUT);
  pinMode(RedSignal, OUTPUT);
  
  pinMode(redpin, OUTPUT);
  pinMode(yellowpin, OUTPUT);
  pinMode(greenpin, OUTPUT);

  pinMode(stopPin, INPUT);
  pinMode(Stopping, OUTPUT);
}

void loop(void) {
  Serial.println("Stop Pin: " + String(digitalRead(stopPin)));
  Serial.println(digitalRead(trackerpin));
  if(digitalRead(BusyPin) == 1){
    digitalWrite(redpin, LOW);
    digitalWrite(yellowpin, HIGH);
    digitalWrite(greenpin, LOW);
  }else if (digitalRead(stopPin) == 1){
    digitalWrite(redpin, HIGH);
    digitalWrite(yellowpin, LOW);
    digitalWrite(greenpin, LOW);
    digitalWrite(Stopping, HIGH);
  } else{
    digitalWrite(redpin, LOW);
    digitalWrite(yellowpin, LOW);
    digitalWrite(greenpin, HIGH);
  }
  if(digitalRead(trackerpin) == 1){
    DoColor();
  }else{
    digitalWrite(RedSignal, LOW);
    digitalWrite(BlueSignal, LOW);
    digitalWrite(GreenSignal, LOW);
  }
  if(digitalRead(stopPin) == 0){
    digitalWrite(Stopping, LOW);
  }
}

void DoColor(){
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);


  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.println(" ");
  test(r, g, b);
}

void test(unsigned int R, unsigned int G, unsigned int B){
  if(R > G && R > B){
    Serial.println("Red");
    digitalWrite(RedSignal, HIGH);
    digitalWrite(BlueSignal, LOW);
    digitalWrite(GreenSignal, LOW);
  } else if(G > B && G > R){
    Serial.println("Green");
    digitalWrite(RedSignal, LOW);
    digitalWrite(BlueSignal, LOW);
    digitalWrite(GreenSignal, HIGH);

  } else if(B > R && B > G){

    Serial.println("Blue");
    digitalWrite(RedSignal, LOW);
    digitalWrite(BlueSignal, HIGH);
    digitalWrite(GreenSignal, LOW);

  }
  
  else{
    Serial.println("Other");
    digitalWrite(RedSignal, LOW);
    digitalWrite(BlueSignal, LOW);
    digitalWrite(GreenSignal, LOW);
  }
}
