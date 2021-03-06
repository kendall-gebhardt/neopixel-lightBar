// Program to run a Neopixel strip with input buttons to control brightness and temperature
// Bonus party buttons also included

#include <Adafruit_NeoPixel.h>

// Define input pins
#define BRIGHT_UP 2
#define BRIGHT_DOWN 3
#define WARM_UP 5
#define WARM_DOWN 6
#define PARTY1 4
#define PARTY2 7

// Define variables for this setup and initialise the strip object
#define LED_PIN A0
#define LED_COUNT 118 

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define some variables to maintain the state of the strip
int brightness = 4;   // Range from 0 (almost off) to 5 (full brightness)
int warmth = 5;       // Range from 0 (cool) to 5 (warm)
bool party1 = false;
bool party2 = false;
int brightupstate = HIGH;
int brightdownstate = HIGH;
int warmupstate = HIGH;
int warmdownstate = HIGH;
int party1state = HIGH;
int party2state = HIGH;

void setup() {
  // This runs once when the strip is turned on
  Serial.begin(9600);
  pinMode(BRIGHT_UP, INPUT_PULLUP);
  pinMode(BRIGHT_DOWN, INPUT_PULLUP);
  pinMode(WARM_UP, INPUT_PULLUP);
  pinMode(WARM_DOWN, INPUT_PULLUP);
  pinMode(PARTY1, INPUT_PULLUP);
  pinMode(PARTY2, INPUT_PULLUP);
  
  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels
  strip.setBrightness(255); // Set BRIGHTNESS to upper limit -- this will be dimmed later
  updateStrip();
}

void loop() {
  // Cycle through checking the buttons for input, making sure to only catch the button release
  // The pin uses the internal pullup and the buttons are wired to connect the pins to ground
 
  // Check brightness pins and adjust on button release
  if (brightupstate != digitalRead(BRIGHT_UP)){ 
    brightupstate = digitalRead(BRIGHT_UP);
    if (brightupstate == HIGH){
      incrementBrightness(1);
    }
  }
  if (brightdownstate != digitalRead(BRIGHT_DOWN)){ 
    brightdownstate = digitalRead(BRIGHT_DOWN);
    if (brightdownstate == HIGH){
      incrementBrightness(-1);
    }
  }
  
  // Check warmth pins and adjust on button release
  if (warmupstate != digitalRead(WARM_UP)){ 
    warmupstate = digitalRead(WARM_UP);
    if (warmupstate == HIGH){
      incrementWarmth(1);
    }
  }
  if (warmdownstate != digitalRead(WARM_DOWN)){ 
    warmdownstate = digitalRead(WARM_DOWN);
    if (warmdownstate == HIGH){
      incrementWarmth(-1);
    }
  }

  // Check party pins and activate on button release
  if (party1state != digitalRead(PARTY1)){ 
    party1state = digitalRead(PARTY1);
    if (party1state == HIGH){
      initiateParty1();
    }
  }
  if (party2state != digitalRead(PARTY2)){ 
    party2state = digitalRead(PARTY2);
    if (party2state == HIGH){
      initiateParty2();
    }
  }
  
}

void incrementBrightness(int increment){
  // Update the brightness variable and update strip
  brightness = brightness + increment;
  
  // Clamp brightness between 0 and 5
  if (brightness > 5){brightness = 5;}
  if (brightness < 0){brightness = 0;}

  updateStrip();
}

void incrementWarmth(int increment){
  // Update the warmth variable and set the warmth

  warmth = warmth + increment;

  // Clamp warmth between 0 and 5
  if (warmth > 5){warmth = 5;}
  if (warmth < 0){warmth = 0;}

  updateStrip();
}

void updateStrip(){
  // Helper function to set the colors by warmth and brightness then update the LEDs
  // Some reference colors:
  // candle = (255, 147, 41)
  // tungstenWhite = (255, 197, 143)
  // warmWhite = (253, 244, 220)
  // brightWhite = (255, 255, 255)
  // coolWhite = (64, 156, 255)

  int red = 255;
  int green = map(warmth, 0, 5, 255, 160);
  int blue = map(warmth, 0, 5, 255, 75);

  int brightnessmodifier = map(brightness, 0, 5, 0, 100); // Map the brightness variable to a percentage
  red = round(red * brightnessmodifier / 100);
  green = round(green * brightnessmodifier / 100);
  blue = round(blue * brightnessmodifier / 100);
  
  uint32_t stripcolor = strip.Color(red, green, blue);

  strip.fill(stripcolor);
  strip.show();

  // Serial log for debugging
  Serial.print("Showing warmth ");
  Serial.print(warmth);
  Serial.print(" with colors (");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.print(blue);
  Serial.print(") with brightness ");
  Serial.println(brightness);

  delay(500); // Wait a bit to allow the user to release the button
}

void initiateParty1(){
  // Enter this loop when the party button is pressed and remain there until the button is pressed again 
  // This rainbow method was adapted from the Adafruit NeoPixel strandtest demo
  Serial.println("Activating party 1");
  party1 = true;
  delay(500); // Allow the user to release the button
  long firstPixelHue = 0;
  while (party1){
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(10);  // Pause for a moment
    firstPixelHue += 265;

    // Check the party1 button and exit on release
    if (party1state != digitalRead(PARTY1)){ 
      party1state = digitalRead(PARTY1);
      if (party1state == HIGH){
        party1 = false;
      }
    }
  }
  // Reset the strip to the last white value upon exiting
  Serial.println("Deactivating party1");
  updateStrip();
}

void initiateParty2(){
  // Enter this loop when the party button is pressed and remain there until the button is pressed again 
  // This rainbow method was adapted from the Adafruit NeoPixel strandtest demo
  Serial.println("Activating party2");
  party2 = true;
  delay(500); // Allow the user to release the button
  long firstPixelHue = 0;
  int oneBar = round(strip.numPixels() / 2);
  while (party2){
    strip.clear();
    strip.show();
    delay(15);
    for(int i=0; i<oneBar; i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / oneBar);
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      strip.setPixelColor(i + oneBar, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(15);  // Pause for a moment
    firstPixelHue += 265 * 3;
    // Check the party2 button and exit on release
    if (party2state != digitalRead(PARTY2)){ 
      party2state = digitalRead(PARTY2);
      if (party2state == HIGH){
        party2 = false;
      }
    }
  }
  // Reset the strip to the last white value upon exiting
  Serial.println("Deactivating party2");
  updateStrip();
}
