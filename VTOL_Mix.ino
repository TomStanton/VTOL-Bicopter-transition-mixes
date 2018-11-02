#include "PWM.hpp"
#include <Servo.h>
#include <Wire.h>

////////// Variables //////////
int throttleIN;
int aileronIN;
int elevatorIN;
int rudderIN;
int tiltIN;

int aileronFADE;
int rudderFADE;
int elevatorFADE;

int throttleleft;
int throttleright;
int tiltservo;

////////// Input Pins //////////
PWM ch1(0); // Throttle
PWM ch2(2); // Aileron
PWM ch3(3); // Elevator
PWM ch4(7); // Rudder
PWM ch5(1); // Tilt

////////// Outputs //////////
Servo throttleleftoutput; 
Servo throttlerightoutput; 
Servo tiltservooutput;

int throttleleftout;
int throttlerightout;
int tiltservoout;


void setup() {

  Serial.begin(115200);
  
  ch1.begin(true);
  ch2.begin(true);
  ch3.begin(true);
  ch4.begin(true);
  ch5.begin(true);
    
  ////////// Output Pins //////////
  throttleleftoutput.attach(5);   // throttleleft
  throttlerightoutput.attach(9);  // throttleright
  tiltservooutput.attach(6);      // tiltservo
}


void loop() {

  ////////// Read Inputs //////////
  aileronIN = ch2.getValue() - 1500; // 1500 is the mid point
  elevatorIN = ch3.getValue() - 1500;
  rudderIN = ch4.getValue() - 1500;
  tiltIN = ch5.getValue() - 1500;
   
  if (ch1.getValue() > 1000 && ch1.getValue() < 2000) {
    throttleIN = ch1.getValue() - 1500;
  } else {
    throttleIN = - 500;
  }
   
  ////////// Transition fades //////////
  tiltIN = constrain(tiltIN, -500, 500);

  //Aileron
  aileronFADE = aileronIN * (1 - ((tiltIN + 500) * 0.001));

  //Rudder
  rudderFADE = rudderIN * ((tiltIN + 500) * 0.001);

  //Elevator
  elevatorFADE = elevatorIN * (1 - ((tiltIN + 500) * 0.001));

  ////////// Mixing //////////

  throttleleft = throttleIN + (aileronFADE * 0.2) - (rudderFADE * 0.2) + 1500;
  throttleright = throttleIN - (aileronFADE * 0.2) + (rudderFADE * 0.2) + 1500;
  tiltservo = - tiltIN - elevatorFADE + 1500;

  ////////// Limits //////////
  throttleleft = constrain(throttleleft, 1000, 2000);
  throttleright = constrain(throttleright, 1000, 2000);
  tiltservo = constrain(tiltservo, 1100, 1900);

  //////////  Outputs //////////
    
  throttleleftoutput.writeMicroseconds(throttleleft);
  throttlerightoutput.writeMicroseconds(throttleright);
  tiltservooutput.writeMicroseconds(tiltservo);

  delay(20);
}
