/*
Arduino RC Controller
*/
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "config.h"

#define BAUD 9600
#define CE 9
#define CSN 10
#define LOWV 3.0
#define VDIV 2.0 // 1/n divider
#define LED_SET 8
#define LED_BAT 7
#define BATT A7
#define POT_SCL A0
#define BUT_UP 3
#define BUT_DN 2

RF24 radio(CE, CSN); // nRF24L01 (CE, CSN)
byte set_max = 255;
byte set_min = 0;
bool set_max_flag = false;
bool set_min_flag = false;

void setup()
{
    Serial.begin(9600);

    pinMode(BUT_UP, INPUT_PULLUP); // Button UP
    pinMode(BUT_DN, INPUT_PULLUP); // Button DOWN
    pinMode(LED_BAT, OUTPUT);      // Low Battery LED
    pinMode(LED_SET, OUTPUT);      // Setpoint LED

    digitalWrite(LED_BAT, HIGH);
    digitalWrite(LED_SET, HIGH);
    delay(500); // Test LED on boot
    digitalWrite(LED_BAT, LOW);
    digitalWrite(LED_SET, LOW);

    radio.begin();
    radio.openReadingPipe(1, ADDRESS1);
    radio.openWritingPipe(ADDRESS2);
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening();

    Serial.println("CTRL Init OK");
}
void loop()
{
    // Check incoming data
    if (radio.available())
    {
        byte val;
        radio.read(&val, sizeof(byte));

        // update setpoints if flag
        if (set_max_flag)
        {
            set_max = val;
            set_max_flag = false;
        }
        if (set_min_flag)
        {
            set_min = val;
            set_min_flag = false;
        }

        // update setpoint led
        digitalWrite(LED_SET, val <= set_min || val >= set_max);

        // thresh & scale value
        if (val < set_min) val = set_min;
        if (val > set_max) val = set_max;
        val = map(val, set_min, set_max, 0, 255);
        val = map(analogRead(POT_SCL), 0, 1023, 0, val);
        // Send data
        radio.stopListening();
        radio.write(&val, sizeof(val));
        radio.startListening();
    }

    // Setpoint
    if (digitalRead(BUT_UP) == LOW && !set_max_flag)
    {
        set_max_flag = true;
        set_min_flag = false;
    }
    if (digitalRead(BUT_DN) == LOW && !set_min_flag)
    {
        set_min_flag = true;
        set_max_flag = false;
    }

    // Check battery
    float voltage = analogRead(BATT) * (5.00 / 1023.00) * VDIV;
    digitalWrite(LED_BAT, voltage < LOWV); // HIGH if true
}
