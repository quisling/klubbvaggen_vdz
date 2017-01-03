#include <Adafruit_NeoPixel.h>
 
#define PIN      6
#define N_LEDS 301
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
 
int wait = 10;
int mode = 1;
uint32_t coloor = strip.Color(255, 0, 0);
int val = 0;
int currentLed = 0;
int brightPin = 2;
int colorPin = 3;
int color_value = 0;
const int  interruptPin = 3;
const int mode_max = 5;
bool modeHelper = false;
int colorval = 0; 
 
 
void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), increase, RISING);
  strip.begin();
}
 
void loop() {
  val = analogRead(brightPin);
  strip.setBrightness(val/4);
  color_value = analogRead(colorPin);
  coloor = Wheel(color_value/4);
  switch (mode){
      case 1:
      //Permanent color
        for (int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, coloor);
        }
        strip.show();
        break;
      case 2:
      //Stroboscope
        for (int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, coloor);
        }
        strip.show();
        delay(20);
      for (int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
        strip.show();
        delay(val/4);
      break;
      case 3:
        //Colorflow
        if(colorval == 255){
          colorval == 0;
        }else{
          colorval++;
        }
        for (int i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(colorval));
        }
        strip.show();
        delay(val/4);
        break;
    case 4:
      //rainbow-flow
      if (currentLed >= strip.numPixels())
        currentLed = strip.numPixels() - 255;
      for (int i=0; i<strip.numPixels(); i++) {
        int input_led = i+currentLed;
        if (input_led >255 ){
          input_led = input_led - 255;
        }
        strip.setPixelColor(i, Wheel(input_led));
      }
      strip.show();
      currentLed++;
      break;
    case 5:
      for (int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(i));
        strip.show();
      }
      delay(2000);
      break;
    }
}
 
static void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+10; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-10, 0); // Erase pixel a few steps back
      strip.show();
      delay(10);
  }
}
 
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void increase(){
  noInterrupts();
  if (mode > mode_max){
    mode = 1;
  }else{
    mode++;
  }
  for (int i=0; i<strip.numPixels(); i++) {
     strip.setPixelColor(i, Wheel(coloor));
     if (i >= mode){
        strip.setPixelColor(i, strip.Color(0, 0, 0));
     }   
  }
  strip.show();
  delay(1000);
  interrupts();
}
