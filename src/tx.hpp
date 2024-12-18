/*
DIY Arduino based RC Transmitter
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

RF24 radio(CE, CSN); // nRF24L01 (CE, CSN)
byte pot = 1;

void setup()
{
  Serial.begin(9600);

  pinMode(7, OUTPUT); // Low Battery LED
  digitalWrite(7, HIGH);
  delay(500); // Test LED on boot
  digitalWrite(7, LOW);

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
  pot = map(analogRead(A1), 0, 1023, 0, 255);
  radio.write(&pot, sizeof(byte));
  // Check battery
  float voltage = analogRead(A7) * (5.00 / 1023.00) * VDIV;
  digitalWrite(7, voltage < LOWV); // HIGH if true
  // Debug
  Serial.println(pot);
}
