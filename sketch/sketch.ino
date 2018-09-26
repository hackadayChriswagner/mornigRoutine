/*
 * mornigRoutine: Sketch drive CoffeMachine and remote 
 * string of lights with IR Led and DS3231 Module.
 * Version 0.2 September, 2018
 * Copyright 2009 Chris Wertip
 */

/*
 * USE:
   IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
   An IR LED must be connected to Arduino PWM pin 3.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/

#include <IRremote.h>
#include <Servo.h>
#include <Wire.h>
#include "DS3231.h"

#define IR_BPLUS  0xF700FF  // 
#define IR_BMINUS 0xF7807F  // 
#define IR_OFF    0xF740BF  // 
#define IR_ON     0xF7C03F  // 
#define IR_R    0xF720DF  // 
#define IR_G    0xF7A05F  // 
#define IR_B    0xF7609F  // 
#define IR_W    0xF7E01F  // 
#define IR_B1   0xF710EF  // 
#define IR_B2   0xF7906F  // 
#define IR_B3   0xF750AF  // 
#define IR_FLASH  0xF7D02F  // 
#define IR_B4   0xF730CF  // 
#define IR_B5   0xF7B04F  // 
#define IR_B6   0xF7708F  // 
#define IR_STROBE 0xF7F00F  // 
#define IR_B7   0xF708F7  // 
#define IR_B8   0xF78877  // 
#define IR_B9   0xF748B7  // 
#define IR_FADE   0xF7C837  // 
#define IR_B10    0xF728D7  // 
#define IR_B11    0xF7A857  // 
#define IR_B12    0xF76897  // 
#define IR_SMOOTH 0xF7E817  // 

#define Switch 2 // NPN Switch an Pin 2 for disable Servo to avoid jiggering

//SET TIMER:        // Set yout wakeup time 
#define ALstd 21    // hour
#define ALmin 30    // minute
#define ALsec 0     // second 



RTClib RTC;
IRsend irsend;
Servo myservo;      // create servo object to control a servo


void setup()
{
  pinMode(Switch, OUTPUT);      // Pin for Transistor Switch
  digitalWrite(Switch, HIGH);   // Set Pin 2 to HIGH go get def Position
  myservo.attach(11);           // attaches the servo on pin 9 to the servo object
  myservo.write(40);            // get Servo in define start position
  delay(500);
  digitalWrite(Switch, LOW);    // Set Pin 0 to LOW to disable Servo jiggering
  Serial.begin(57600);
  Wire.begin();
  sunrise(IR_ON);               // Init Led Stripe
  sunrise(IR_W);                // set start Color in white
  dimmPlus();                   // set  brightness to max
  sunrise(IR_OFF);              // set chain of lights offline

}

void loop() {
  timer(0, IR_ON);    // Set LED ONLINE
  timer(0, IR_R);     // Set first Color
  
  timer(2, IR_B1);    // Set next Color after SNOOZE
  timer(4, IR_B4);    // Set next Color after SNOOZE
  timer(6, IR_B7);    // Set next Color after SNOOZE
  timer(8, IR_B10);   // Set next Color after SNOOZE
  timer(10, IR_G);    // Set next Color after SNOOZE
  timer(12, IR_B2);   // Set next Color after SNOOZE
  timer(14, IR_B5);   // Set next Color after SNOOZE
  timer(16, IR_B8);   // Set next Color after SNOOZE
  timer(18, IR_B11);  // Set next Color after SNOOZE
  timer(20, IR_W);    // Set next Color after SNOOZE
  timer(22, IR_W);    // Set next Color after SNOOZE
}

void timer(int snoozer, float irCode) {
  DateTime now = RTC.now(); // get actual Time, if matches, than do sunris code 
  if ( ((now.hour() == ALstd) && (now.minute() == (ALmin + (0))) && (now.second() == (ALsec + (snoozer)) )) == true ) {
    Serial.println("ZEIT treffer ");
    sunrise(irCode);
    if (snoozer == 22) {  // snoozer is 22 form loop
      getCoffe(); // trigger buttons to get Senseo-Coffe
      lastchance();
    }
  }
}

void lastchance() {
  do {
    Serial.println("lastchance!!!");
    sunrise(IR_OFF);
    delay(1000);
    sunrise(IR_ON);
    delay(1000);
    dimmPlus();
    sunrise(IR_FADE);
    delay(60000);
    dimmMinus();
    delay(1000);
    for (int i = 0; i < 1000; i++) {
      sunrise(IR_B);
      sunrise(IR_OFF);
      delay(500);
      sunrise(IR_ON);
      delay(1000);
    }
  } while (false);
}

void sunrise(float irCode) {    //basic function for sending code to stripe 3 times
  for (int i = 0; i < 4; i++) {
    irsend.sendNEC(irCode, 32);
    Serial.println("TRANS1 NEC: IR (sunrise code)");
    delay(50);
  }
}

void dimmPlus() {   // dim lighten Button
  for (int i = 0; i < 13; i++) {
    sunrise(IR_BPLUS);
    Serial.println("Plus");
    delay(50);
  }
}

void dimmMinus() {    // dim bedim Button
  for (int i = 0; i < 13; i++) {
    sunrise(IR_BMINUS);
    Serial.println("Minus");
    delay(50);
  }
}


void getCoffe() {       // Program for coffe
  Serial.println("Make Coffe now :)");
  Serial.println("Bail water... ");
  buttonCM();
  delay(110000);        // wait 110000 microsec to boil water
  buttonGet2Cup();      // move servo to get coffe
  Serial.println("Hit coffe button");
  Serial.println("Coffe is comming soon.......");
}


void buttonCM() { // trigger Cofeemachine Button
  digitalWrite(Switch, HIGH);
  delay(100);
  myservo.write(15);
  delay(500);
  myservo.write(40);
  delay(500);
  digitalWrite(Switch, LOW);
}

void buttonGet2Cup() { // trigger Cofeemachine for 2 Cups Cofffe
  digitalWrite(Switch, HIGH);
  delay(100);
  myservo.write(78);
  delay(350);
  myservo.write(40);
  delay(500);
  digitalWrite(Switch, LOW);
}

