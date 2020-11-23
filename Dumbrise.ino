// WAKE UP TO LIGHT ALARM
// Right now just keeping this dumb.
// As soon as it gets power, the wake-up animation plays
// 24 hours later it plays the animation again
// So you "set" the alarm by plugging in the power at the same time you want it to go off tomorrow

// Here are the current steps:
// Fade from 0 to full brightness of white channel
  // Eventually I'd like to change colors over the course of this to be more like a sunrise
  // Start with low-intensity red, go through yellow and finish as bright as possible
  // I suspect this would be easier to achieve in an HSV space, but I haven't figured out which library to grab nor how to use it
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

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
}

#define NUM_STEPS 255 

unsigned long hoursToMinutes(unsigned long hours) {
  return hours * 60;
}

unsigned long minutesToSeconds(unsigned long minutes) {
  return minutes * 60;
}

unsigned long secondsToMilliseconds(unsigned long minutes) {
  return minutes * 1000; 
}

// given a specific number of minutes, return the number of milliseconds required for one interval
unsigned long minToIntervalMS(unsigned long minutes, unsigned long numSteps)
{
  unsigned long result = secondsToMilliseconds(minutesToSeconds(minutes)) / numSteps;
}

void loop() {

    //The value in parenthesis is the delay between increments
    // If I make the delay = 3,515 milliseconds I think it would increment 256 times
    // 3,515 * 256 = about 15 minutes (my ballpark of how long I want it to take)
    // Currently that delay value controls the speed of the fade down as well, but I'd like to decouple so fade down could be faster

    // first argument is number of minutes for fade up, and second argument is number of minutes to fade down
    pulseWhite(15, 3);

    // This green wipe effect I'm just using as a feedback mechanism.
    // I think I could make it serve as a countdown timer
    // If I make the delay between steps = 2,880,00 milliseconds, doing that 30 times (once for each pixel) would give me 24 hours
    // I could adjust that a little to accommodate the time the ~15 minutes the white pulse animation takes.
    // Fill along the length of the strip in various colors...

    colorWipe(strip.Color(0,10,0), hoursToMinutes(24));

}

void pulseWhite(unsigned long fadeUpMinutes, unsigned long fadeDownMinutes) {
  unsigned long delayms = minToIntervalMS(fadeUpMinutes, NUM_STEPS);

  for(int j=0; j <= NUM_STEPS; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(delayms);
  }

  delayms = minToIntervalMS(fadeDownMinutes, NUM_STEPS);

  for(int j= NUM_STEPS; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(delayms);
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
