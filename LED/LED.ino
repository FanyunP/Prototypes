#include <Adafruit_DotStar.h>
#include <SPI.h>

//LED
#define DATAPIN    4
#define CLOCKPIN   5
#define NUMPIXELS 75 // Number of LEDs in strip
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
int      head  = 0, tail = 0, lasthead = 0,lasttail = 0,on = 0, off = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

int lightSpeed = 0;
const int infraredPin = A0;
int lastInfraredValue = 0;
int currentInfraredValue = 0;
int infraredValue = 0;

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 500;

bool ledStart = false;
bool reset = false;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  head= on;
  tail = off;
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

void loop() {
  // put your main code here, to run repeatedly:
  lastInfraredValue = currentInfraredValue;
  currentInfraredValue = infraredValue;
  infraredValue = digitalRead(infraredPin);
//  Serial.print("infraredValue: ");
//  Serial.println(infraredValue);

   if(lastInfraredValue!=currentInfraredValue){
    Serial.print("currentInfraredValue = ");
    Serial.println(currentInfraredValue);
    if(currentInfraredValue == 0){
//      if(infraredValue == 0){
      ledStart  = true;
      Serial.println("START!");
      lastDebounceTime = millis();
    }
  }

  if(ledStart){
      lights(lightSpeed);
      lasthead = head;
      lasttail = tail;
    }
  else{
        for(int i = lasttail; i < lasthead; i++){
//          Serial.print("id: ");
//          Serial.println(lasthead - i);
          strip.setPixelColor(i, 0); 
          strip.show();
          }          
      head = on;
      tail = off;
      }
}

void lights(int breakTime){
  //LED
  strip.setPixelColor(head, 0,255,0); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(breakTime);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;             //   Yes, reset to red
  }
  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index

  if((millis() - lastDebounceTime) > debounceDelay){
    ledStart = false;
    Serial.println("END!");
    }
  }

