/*
 * SimpleSender.cpp
 *
 *  Demonstrates sending IR codes in standard format with address and command
 *  An extended example for sending can be found as SendDemo.
 *
 *  Copyright (C) 2020-2022  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 *  MIT License
 */
#include <Arduino.h>

#if !defined(ARDUINO_ESP32C3_DEV) // This is due to a bug in RISC-V compiler, which requires unused function sections :-(.
#define DISABLE_CODE_FOR_RECEIVER // Disables static receiver code like receive timer ISR handler and static IRReceiver and irparams data. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not required.
#endif
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> // include the library
// #define IR_SEND_PIN
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    IrSender.begin();
}

/*
 * Set up the data to be sent.
 * For most protocols, the data is build up with a constant 8 (or 16 byte) address
 * and a variable 8 bit command.
 * There are exceptions like Sony and Denon, which have 5 bit address.
 */
#define BRIGTHNESS_ADDR 0x01
#define R_ADDR 0x02
#define G_ADDR 0x03
#define B_ADDR 0x04
#define PIXEL 0x05
#define ANIM_ADDR 0x06 // Animation mode. Allows for individual control of pixels
#define UPPER_LIM 50
#define LOWER_LIM 15
const uint8_t signal_delay = 10;

uint8_t brightness = LOWER_LIM;
bool direction_up = true;
uint8_t brightness_step = 0x02;

uint8_t red = 30;
uint8_t green = 0;
uint8_t blue = 0;

void breathe()
{
    if (direction_up)
    {
        if (brightness + brightness_step > UPPER_LIM)
        {
            direction_up = false;
        }
        else
        {
            brightness += brightness_step;
        }
    }
    else if (direction_up == false)
    {
        if (brightness - brightness_step < LOWER_LIM)
        {
            direction_up = true;
        }
        else
        {
            brightness -= brightness_step;
        }
    }
}

void sendRGB(uint8_t r, uint8_t g, uint8_t b)
{
    IrSender.sendNEC(R_ADDR, r, 0);
    delay(signal_delay);
    IrSender.sendNEC(G_ADDR, g, 0);
    delay(signal_delay);
    IrSender.sendNEC(B_ADDR, b, 0);
    delay(signal_delay);
}

void AnimationMode(uint8_t mode)
{
    IrSender.sendNEC(ANIM_ADDR, mode, 0);
    delay(signal_delay);
}

uint8_t pixel = 0;
void SendPixel(uint8_t pixel)
{

    IrSender.sendNEC(PIXEL, pixel, 0);
    delay(signal_delay);
}
void loop()
{

    if (digitalRead(D1) == LOW)
    {
        AnimationMode(1);
    }
    if (digitalRead(D2) == LOW)
    {
        pixel += 1;
        SendPixel(5);
    }
    if (digitalRead(D3) == LOW)
    {
        green += 10;
    }

    sendRGB(red, green, blue);
    // IrSender.sendNEC(BRIGTHNESS_ADDR, 50, 0);
    // colors();
    Serial.println("Sent");
    delay(signal_delay); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}
