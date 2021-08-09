#include <AccelStepper.h>
#include <DMXSerial.h>


//Defining pins
#define STEPPIN 5 // Pulse or step pin
#define DIRPIN 6   // Direction pin


//defining terms


int Motor1speed = 0;
int Motor1position = 0;
int speedmin = 0; //pulses per second
int speedmax = 4000;  //pulses per second
AccelStepper step1(1, STEPPIN, DIRPIN); //stepper1 definition 


void setup() {               

  DMXSerial.init(DMXReceiver);
  DMXSerial.write(1, 0); // DMX channel 1 = speed
  DMXSerial.write(2, 0); // DMX channel 2 = direction

  step1.setMaxSpeed (speedmax);  
  step1.setSpeed(0);
  step1.setAcceleration(1000);

  pinMode(STEPPIN, OUTPUT);
  pinMode(DIRPIN, OUTPUT);


}

void loop() {
  if(DMXSerial.read(2)<127) {
    digitalWrite(DIRPIN, LOW); // clockwise direction
    // map DMX value to speed value
    Motor1speed = map((DMXSerial.read(1)),0,255,speedmin,speedmax);
    step1.setSpeed(Motor1speed);
    step1.runSpeed();
  }
  else if (DMXSerial.read(2)>127) {
    digitalWrite(DIRPIN, HIGH); //  anticlockwise direction
    // map DMX value to speed value
    Motor1speed = map((DMXSerial.read(1)),0,255,speedmin,speedmax);
    step1.setSpeed(Motor1speed);
    step1.runSpeed();
  }

}
