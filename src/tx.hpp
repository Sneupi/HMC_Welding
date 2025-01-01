/*
DIY Arduino based RC Transmitter
*/
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "config.h"

#define BAUD 9600
#define CE 8
#define CSN 7
#define LOWV 3.0
#define VDIV 2.0 // 1/n divider
#define LED 2
#define BATT A0
#define POT A1

RF24 radio(CE, CSN); // nRF24L01 (CE, CSN)
byte pot = 1;

void setup()
{
  Serial.begin(9600);

  pinMode(LED, OUTPUT); // Low Battery LED
  digitalWrite(LED, HIGH);
  delay(500); // Test LED on boot
  digitalWrite(LED, LOW);

  radio.begin();
  radio.openWritingPipe(ADDRESS1);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  Serial.println("TX Init OK");
}
void loop()
{
  // Send data
  pot = map(analogRead(POT), 0, 1023, 0, 255);
  radio.write(&pot, sizeof(byte));
  // Check battery
  float voltage = analogRead(BATT) * (5.00 / 1023.00) * VDIV;
  digitalWrite(LED, voltage < LOWV); // HIGH if true
  // Debug
  Serial.println(pot);
}
