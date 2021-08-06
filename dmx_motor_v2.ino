#include <DMXSerial.h>
#include <AccelStepper.h>

//Define pins
int Stepper1Pulse = 3;  // Pulse pin
int Stepper1Direction = 2;  // Direction pin
   
//Initialize

int Motor1speed = 0; // DMX value received
int speedmin = 0; //pulses per second
int speedmax = 1600;  //pulses per second

//Constructor
AccelStepper stepper1(1, Stepper1Pulse, Stepper1Direction);

const int GreenPin =    9;  // PWM output pin for Green Light = CHECK DMX STATUS
#define GreenDefaultLevel   0

void setup() { 
   
  DMXSerial.init(DMXReceiver); // DMX 
  DMXSerial.write(1, 0);
  stepper1.setMaxSpeed (speedmax);  
  stepper1.setSpeed(0);
  stepper1.setAcceleration(1000);
  pinMode(Stepper1Pulse, OUTPUT);
  pinMode(Stepper1Direction, OUTPUT);
  digitalWrite(Stepper1Pulse, LOW);
  digitalWrite(Stepper1Direction, LOW);
  
  pinMode(GreenPin,   OUTPUT); // digital pin for Green Light as output

}

void loop()
{
   unsigned long lastPacket = DMXSerial.noDataSince(); // Calculate how long no data bucket was received
   if ((lastPacket < 5000) && (DMXSerial.read(1)>1)) //// read recent DMX values and read value of channel 1 
    {
      analogWrite(GreenPin,   DMXSerial.read(1));
      Motor1speed = map((DMXSerial.read(1)),0,255,speedmin,speedmax); // map DMX value to speed value
      stepper1.setSpeed(Motor1speed); // set speed 
      stepper1.runSpeed(); // run stepper
          
    }
    else{
       //Disable stepper when no data was received since 5 seconds or more.
        stepper1.stop();//stop stepper
        stepper1.disableOutputs(); //disable power
    }
}
