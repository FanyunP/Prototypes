#include <AccelStepper.h>
#include <SPI.h>
AccelStepper stepper;

const int potentiometerPin = A0;
const int infraredPin = A1;

int PotentiometerValue = 0;
int lastInfraredValue = 0;
int currentInfraredValue = 0;
int infraredValue = 0;
int maxPenguinAcc = 250;

int penguinPos = 800;
int resetPos = 0;

unsigned long timebreak = 800;
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
  int moterAcc = map(PotentiometerValue,0,1023,0,maxPenguinAcc);
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
    motorMove(moterAcc,penguinPos);
    }
  else{
      stepper.setMaxSpeed(500.0);
      stepper.setAcceleration(moterAcc);
      stepper.runToNewPosition(resetPos);
    }
}

void motorMove(int acc,int pos){
 // Serial.println("START!");
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(acc);
  stepper.runToNewPosition(pos);
//  if((millis() - lastDebounceTime) > debounceDelay){
     if (stepper.distanceToGo() == 0)
    {
    motorStart = false;
    Serial.println("END!");
    delay(timebreak);
    }
  }


