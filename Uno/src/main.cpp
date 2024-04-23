
#define DECODE_NEC // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "PinDefs.h"
#include <IRremote.hpp> // include the library
#define PIN 6           // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 6
#define BRIGTHNESS_ADDR 0x01
#define R_ADDR 0x02
#define G_ADDR 0x03
#define B_ADDR 0x04
#define PIXEL_ADDR 0x05
#define ANIM_ADDR 0x06 // Animation mode. Allows for individual control of pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint8_t IR_R = 255;
uint8_t IR_G = 255;
uint8_t IR_B = 255;

uint8_t animation_mode = 0x00;
uint8_t pixel = 0;

void setup()
{
  Serial.begin(115200);

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  pixels.begin();
}
void loop()
{

  if (IrReceiver.decode())
  {
    IrReceiver.resume();

    if (IrReceiver.decodedIRData.address == R_ADDR)
    {
      IR_R = IrReceiver.decodedIRData.command;
    }
    if (IrReceiver.decodedIRData.address == G_ADDR)
    {
      IR_G = IrReceiver.decodedIRData.command;
    }
    if (IrReceiver.decodedIRData.address == B_ADDR)
    {
      IR_B = IrReceiver.decodedIRData.command;
    }
    if (IrReceiver.decodedIRData.address == BRIGTHNESS_ADDR)
    {
      pixels.setBrightness(IrReceiver.decodedIRData.command);
    }
    if (IrReceiver.decodedIRData.address == ANIM_ADDR)
    {
      animation_mode = IrReceiver.decodedIRData.command;
    }
    if (IrReceiver.decodedIRData.address == PIXEL_ADDR)
    {
      pixel = min(IrReceiver.decodedIRData.command, NUMPIXELS - 1);
    }

    if (IrReceiver.isIdle())
    {

      Serial.println(animation_mode);
      Serial.println(pixel);
      Serial.print(IR_R);
      Serial.print(",");
      Serial.print(IR_R);
      Serial.print(",");
      Serial.print(IR_R);
      Serial.println();

      if (animation_mode == 1 && pixel != 0)
      {
        Serial.println("Animation mode");
        pixels.setPixelColor(pixel, pixels.Color(IR_R, IR_G, IR_B));
        pixels.show();
      }
      else
      {
        Serial.println("Normal mode");
        for (int i = 0; i < NUMPIXELS; i++)
        {
          pixels.setPixelColor(i, pixels.Color(IR_R, IR_G, IR_B));
        }
      }

      if (pixels.getBrightness() > 60)
      {
        pixels.setBrightness(60);
      }
      pixels.show();
    }
  }
  // els
}