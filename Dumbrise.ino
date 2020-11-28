// WAKE UP TO LIGHT ALARM
// Right now just keeping this dumb.
// As soon as it gets power, the wake-up animation plays
// 24 hours later it plays the animation again
// So you "set" the alarm by plugging in the power at the same time you want it to go off tomorrow

// Here are the current steps:
// Fade from 0 to full brightness of white channel
// Pause at full brightness for specific count of milliseconds -I haven't figured this out-
// Fade back down to zero brightness (ideally faster than it does now)
// Green wipe animation - I'm just using this as a feedback mechansim atm.
// I think it can also be a way to visualize the 23-ish hour delay

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN     D4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  30

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 255 // Set BRIGHTNESS to 100% (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  Serial.begin(9600);//I think this is right, but gives error "the selected serial port _ does not exist or your board is not connected"
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
  
}

#define NUM_STEPS 255 

/* this is a function that given the number of hours in an argument, returns the number of minutes equivalent
  to those hours */
unsigned long hoursToMinutes(unsigned long hours) {
  return hours * 60; 
}                    

// this is a function, that given the number of minutes, converts the results into seconds
unsigned long minutesToSeconds(unsigned long minutes) {
  return minutes * 60; // this establishes two big variables called "minutesToSeconds" and "minutes"
}

// this is a function that given the number of seconds, converts the result to milliseconds 
unsigned long secondsToMilliseconds(unsigned long seconds) {
  return seconds * 1000;  // this establishes two big variables called "secondsToMilliseconds" and "minutes"?
}                         // Are we re-establishing "minutes" here or is something else going on? I expected to see "seconds"

// given a specific number of minutes, return the number of milliseconds required for one interval
unsigned long minToIntervalMS(unsigned long minutes, unsigned long numSteps)
{
  return secondsToMilliseconds(minutesToSeconds(minutes)) / numSteps;
}


void loop() {

    // first argument is number of minutes for fade up, and second argument is number of minutes to fade down
    pulseWhite(15, 3);

    // This green wipe effect I'm just using as a feedback mechanism.
    // I think I could make it serve as a countdown timer
    // If I make the delay between steps = 2,880,00 milliseconds, doing that 30 times (once for each pixel) would give me 24 hours
    // I could adjust that a little to accommodate the time the ~15 minutes the white pulse animation takes.
    // Fill along the length of the strip in various colors...

    colorWipe(strip.Color(0,10,0), hoursToMinutes(24));
    //Serial.println(hoursToMinutes); //this won't compile "call of overloaded 'println(long unsigned int (&)(longundigned int))' is ambiguous"

}

void pulseWhite(unsigned long fadeUpMinutes, unsigned long fadeDownMinutes) {
  unsigned long delayms = minToIntervalMS(fadeUpMinutes, NUM_STEPS);
  // Ideally, color would change as part of this animation.
  // Hue would start at 0 and stop at 60. It could simply increment by 1 on each step and stop changing once it reaches 60.
  // Saturation would be 100% thoughout
  // Brightness would start at 0 and reach 255 at or near the end of the sequence
  
    // Serial.println(fadeDownMinutes); //is always 3
    // Serial.println(fadeUpMinutes); //is always 15
    // Serial.println(delayms); //It keeps giving me a value of zero...
    // Serial.println(NUM_STEPS); //is always 255


  for(int j=0; j <= NUM_STEPS; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(delayms);
    // Serial.println(j); //increments from 0 to 255 by +1 really damn fast.
    // Serial.println(NUM_STEPS); //is always 255
    // Serial.println(delayms); //is always returning zero

  }

  delayms = minToIntervalMS(fadeDownMinutes, NUM_STEPS);

  for(int j= NUM_STEPS; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(delayms);
    // Serial.println(j); //decrements from 0 to 255 by -1 really damn fast.
    // Serial.println(NUM_STEPS); //is always coming back as 255
    // Serial.println(delayms); //is always coming back as 0 after an inital line of gibberish:
  }
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, unsigned long minutes) {
  
  unsigned long delayms = minToIntervalMS(minutes, strip.numPixels());

  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(delayms);                        //  Pause for a moment
  }
}
