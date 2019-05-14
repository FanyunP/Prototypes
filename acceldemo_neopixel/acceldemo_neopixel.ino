// Basic demo for accelerometer readings from Adafruit LIS3DH

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_DotStar.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 9

#define NUMPIXELS 280
#define DATAPIN    2
#define CLOCKPIN   3

//ACC
float acc,lastAcc,currentAcc,disAcc;
int lastTimebreak = 150;
int Threshold = 150;

//LED
int head  = 0,tail = -15;// Index of first 'on' and 'off' pixels
int pause = 0;
int smoothSpeed = 1;

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// software SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);
// hardware SPI
//Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS);
// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);
  Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x19)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  
  Serial.print("Range = "); Serial.print(2 << lis.getRange());  
  Serial.println("G");

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  lastTimebreak = Threshold;
  pause = Threshold;

//LED
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  //LED lightEfftect2
//  for(int i = 0 ; i< NUMPIXELS ; i++){
//    strip.setPixelColor(i,255,255,255); 
//    }
}

void loop() {
  Acc();
  float accInt = abs(disAcc);
  accInt = 4.5 * accInt + 5;
  Serial.print("accInt: ");
  Serial.println(accInt);
  if(accInt > 9){
      int timebreak = 50/int(accInt);
      int dir = disAcc/abs(disAcc);
      int lightcolor = map(accInt,5,50,0,255);
//      Serial.print("timebreak: ");
//      Serial.print(timebreak);
//      Serial.print("  lightcolor: ");
//      Serial.println(lightcolor);
//      Serial.print("   dir: ");
//      Serial.println(dir);
      lastTimebreak = timebreak;
      lightEffect(timebreak,head,lightcolor,255,255);
      lightEffect2(timebreak,30,lightcolor,255,255);
      //lightEffect3(255,lightcolor,lightcolor,timebreak);
    }
    else{
      smooth(Threshold,smoothSpeed);
      lightEffect(pause,head,255,255,255);
      //lightEffect2(pause,255,255,255);
     //lightEffect3(255,255,255,100);
      }
}

void Acc(){
   //Acc
  lis.read();      // get X Y and Z data at once
  // Then print out the raw data
  /* Or....get a new sensor event, normalized */ 
  sensors_event_t event; 
  lis.getEvent(&event);

  disAcc = currentAcc - lastAcc;
  lastAcc = currentAcc;
  currentAcc = acc;
  acc = sqrt(sq(event.acceleration.x) +  sq(event.acceleration.y) +  sq(event.acceleration.z));
//  Serial.print("Dis: ");
//  Serial.println(disAcc);
  }

void smooth(int maxTime, int sped){
  if(maxTime > lastTimebreak){
    lastTimebreak += sped;
    pause = lastTimebreak;
    }
  }

void lightEffect(int breaktime,int start, int r,int g, int b){
  //LED
  strip.setPixelColor(start,r,g,b); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(breaktime);                        // Pause 20 milliseconds (~50 FPS)
    if(++head >= NUMPIXELS) { 
       head = 0;  
      }
   if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
  }

void lightEffect2(int breaktime,int num,int r,int g, int b){
  int id [num];
  for(int i = 0; i < num; i++){
    id[i] = random(NUMPIXELS);
    strip.setPixelColor(id[i],r,g,b); 
    }
  strip.show(); 
  delay(breaktime);
  for(int i = 0; i < num; i++){
  strip.setPixelColor(id[i],0,0,0); 
  }
  strip.show(); 
  delay(breaktime);
  }

void lightEffect3(int r,int g, int b, int breaktime){
  int id = random(NUMPIXELS);
  strip.setPixelColor(id,0,0,0);  
  strip.show(); 
  delay(breaktime);
  strip.setPixelColor(id,r,g,b); 
  strip.show(); 
  //delay(breaktime);
  }

