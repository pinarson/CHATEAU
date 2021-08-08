#include <DMXSerial.h>
#include <AccelStepper.h>
#include <Arduino.h>

//Define pins
#define STEPPIN 12  
#define DIRPIN 11 
#define ENAPIN 10
#define TYPE_INTERFACE_MOTEUR 1 
#define GREENLED 9
#define REDLED 6

//Initialize

int Stepper1speed = 0; // DMX value received correnponds to the speed of stepper
int speedmin = 0; //pulses per second
int speedmax = 1600;  //pulses per second

//Constructor
AccelStepper stepper1 = AccelStepper(TYPE_INTERFACE_MOTEUR, STEPPIN, STEPPIN);

void Forward();
void Reverse();
void StopAll();


void setup() { 
   
  DMXSerial.init(DMXReceiver);
  DMXSerial.write(1, 0);
  DMXSerial.write(2, 0);
  stepper1.setMaxSpeed (speedmax);  
  stepper1.setSpeed(0);
  stepper1.setAcceleration(1000);
  pinMode(STEPPIN, OUTPUT);
  pinMode(DIRPIN, OUTPUT);
  digitalWrite(STEPPIN, LOW);
  digitalWrite(DIRPIN, LOW);
  // digital pin for Green Light as output
  pinMode(GREENLED, OUTPUT); 
  pinMode(REDLED, OUTPUT); 

}

void loop(){
  // Calculate how long no data bucket was received
  unsigned long lastPacket = DMXSerial.noDataSince(); 
  int CH_one = DMXSerial.read(1);
  int CH_two = DMXSerial.read(2);
  
  //read recent DMX values 
  if ((lastPacket < 5000) && (CH_one>1) && (CH_two<127) ){
    Forward();         
  }
  else if ((lastPacket < 5000) && (CH_one>1) && (CH_two>127) ){
    Reverse();      
  }
  else{
    StopAll(); 
  }
}


void Forward(){
  //write DMX values and assigned value to Green led intensity
  analogWrite(GREENLED,   DMXSerial.read(1));
  digitalWrite(DIRPIN, LOW);// clockwise driection
  Stepper1speed = map((DMXSerial.read(1)),0,255,speedmin,speedmax);
  // set stepper speed 
  stepper1.setSpeed(Stepper1speed);  
  // run stepper
  stepper1.runSpeed();     
}

void Reverse(){
  //write DMX values and assigned value to Green led intensity
  analogWrite(GREENLED,   DMXSerial.read(1));
  digitalWrite(DIRPIN, HIGH);
  // map DMX value to speed value
  Stepper1speed = map((DMXSerial.read(1)),0,255,speedmin,speedmax);
  // set stepper speed 
  stepper1.setSpeed(Stepper1speed);  
  // run stepper
  stepper1.runSpeed();  
}

void StopAll(){
  stepper1.stop();
  //disable power
  stepper1.disableOutputs(); 
}
