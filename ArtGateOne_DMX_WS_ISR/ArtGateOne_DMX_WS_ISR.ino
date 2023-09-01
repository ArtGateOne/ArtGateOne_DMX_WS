/*
  ArtGateOne DMX --> WS v1.0 ISR
*/

#include <lib_dmx.h>  // comment/uncomment #define USE_UARTx in lib_dmx.h as needed
#include <Adafruit_NeoPixel.h>

#define DMX512 (0)         // (250 kbaud - 2 to 512 channels) Standard USITT DMX-512
#define LED_PIN 6          // Defined led pin
#define LED_COUNT 96       // How many NeoPixels are attached to the Arduino 1-170
#define LED_BRIGHTNESS 50  // 1-255
#define LED_SEGMENTS 32


unsigned int DMX_ADDRESS = 1;                               //DMX START ADDRES 1-512 <--! (512 - LED_COUNT)

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
  DMX_ADDRESS--;

  strip.begin();                        // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();                         // Turn OFF all pixels ASAP
  strip.setBrightness(LED_BRIGHTNESS);  //set led brighness

  ArduinoDmx0.set_control_pin(2);  // Arduino output pin for MAX485 input/output control (connect to MAX485-1 pins 2-3)

  ArduinoDmx0.set_rx_address(1);     // set rx1 start address
  ArduinoDmx0.set_rx_channels(512);  // 2 to 2048!! channels in DMX1000K (512 in standard mode) See lib_dmx.h  *** new *** EXPERIMENTAL
  ArduinoDmx0.attachRXInterrupt(frame_received);
  ArduinoDmx0.init_rx(DMX512);

}  //end setup()

void loop() {

}  //end loop()


void frame_received(uint8_t universe)  // Custom ISR: fired when all channels in one universe are received
{
  unsigned DMX_CHANNEL = DMX_ADDRESS;

  if (universe == 0) {  // USART0

    for (unsigned int i = 0; i < LED_COUNT; i++) {  //dmx buffer to ws strip

      byte r = ArduinoDmx0.RxBuffer[DMX_CHANNEL];
      byte g = ArduinoDmx0.RxBuffer[(DMX_CHANNEL + 1)];
      byte b = ArduinoDmx0.RxBuffer[(DMX_CHANNEL + 2)];

      strip.setPixelColor(i, r, g, b);

      for (unsigned int s = 1; s < LED_PER_SEGMENT; s++) {  //SEGMENT COPY COLORS
        i++;
        strip.setPixelColor(i, r, g, b);
      }

      DMX_CHANNEL += 3;
    }
  }
  strip.show();  //send data to WS LED
}  // end of ISR
