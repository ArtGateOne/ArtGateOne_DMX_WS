/*
  ArtGateOne DMX --> WS v1.0 DMXserial library
*/

#include <DMXSerial.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6          // Defined led pin
#define LED_COUNT 96       // How many NeoPixels are attached to the Arduino 1-170
#define LED_BRIGHTNESS 50  // 1-255
#define LED_SEGMENTS 32


unsigned int DMX_ADDRESS = 1;  //DMX START ADDRES 1-512 <--! (512 - LED_COUNT)

unsigned int LED_PER_SEGMENT = (LED_COUNT / LED_SEGMENTS);  //HOW MANY PIXELS PER SEGMENT


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


void setup() {
  //DMX_ADDRESS--;
  DMXSerial.init(DMXProbe);
  DMXSerial.maxChannel(DMX_ADDRESS + (LED_COUNT * 3) - 1);

  strip.begin();                        // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                         // Turn OFF all pixels ASAP
  strip.setBrightness(LED_BRIGHTNESS);  //set led brighness

}  //end setup()


void loop() {

  if (DMXSerial.receive()) {

    unsigned int DMX_CHANNEL = DMX_ADDRESS;

    byte* dmxBuffer = DMXSerial.getBuffer();

    for (unsigned int i = 0; i < LED_COUNT; i++) {  //dmx buffer to ws strip

      byte r = dmxBuffer[DMX_CHANNEL];
      byte g = dmxBuffer[DMX_CHANNEL + 1];
      byte b = dmxBuffer[DMX_CHANNEL + 2];

      strip.setPixelColor(i, r, g, b);

      for (unsigned int s = 1; s < LED_PER_SEGMENT; s++) {  //SEGMENT COPY COLORS
        i++;
        strip.setPixelColor(i, r, g, b);
      }

      DMX_CHANNEL += 3;
    }

    strip.show();  //send data to WS LED
  }
}  //end loop()
