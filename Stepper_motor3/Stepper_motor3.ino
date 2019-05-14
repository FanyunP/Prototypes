#include <AccelStepper.h>
#include <SPI.h>
AccelStepper stepper;

const int potentiometerPin = A0;
const int infraredPin = A1;

int PotentiometerValue = 0;
int lastInfraredValue = 0;
int currentInfraredValue = 0;
int infraredValue = 0;
int maxVelocity = 1000;

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 1000;

bool motorStart = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
  PotentiometerValue = analogRead(potentiometerPin);
  int moterAcc = map(PotentiometerValue,0,1023,0,maxVelocity);
  //Serial.println(moterAcc);
  lastInfraredValue = currentInfraredValue;
  currentInfraredValue = infraredValue;
  infraredValue = digitalRead(infraredPin);
  
  if(lastInfraredValue!=currentInfraredValue){
   // Serial.print("currentInfraredValue = ");
   // Serial.println(currentInfraredValue);
    if(currentInfraredValue == 0){
      motorStart = true;
      Serial.println("START!");
      lastDebounceTime = millis();
    }
  }

  if(motorStart){
    motorMove(moterAcc);
    }
  else{
     stepper.stop();
  //   Serial.println("END!");
    }
 

}

void motorMove(int acc){
 // Serial.println("START!");
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(acc);
  //stepper.runToNewPosition(500);
  //stepper.moveTo(500);
  stepper.run();
  if((millis() - lastDebounceTime) > debounceDelay){
    motorStart = false;
    Serial.println("END!");
    }
  
 // if (stepper.distanceToGo() == 0){
   // motorStart = false;
 //   Serial.println("motorStart: false");
    //}
  }

