/*
Arduino RC Receiver
*/
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include "config.h"

#define BAUD 9600
#define CE 9
#define CSN 10
#define TIMEOUT 500
#define SERVO 2
#define RELAY 3

Servo servo;
RF24 radio(CE, CSN); // nRF24L01 (CE, CSN)
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;
byte pot;

void resetData()
{
    pot = 0;
}

void updateServo()
{
    int angle = map(pot, 0, 255, 0, 180);
    servo.write(angle);
}

void updateRelay()
{
    digitalWrite(RELAY, pot > 0);
}

void setup()
{   
    Serial.begin(BAUD);
    servo.attach(SERVO); // init servo
    pinMode(RELAY, OUTPUT); // init relay

    radio.begin();
    radio.openReadingPipe(0, ADDRESS2);
    radio.setAutoAck(false);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.startListening(); //  Set the module as receiver
    resetData();
    
    updateRelay();
    updateServo();
    Serial.println("RX Init OK");
}

void loop()
{
    // Check lost connection
    currentTime = millis();
    if (currentTime - lastReceiveTime > TIMEOUT)
    {
        // Reset, avoid unwanted behavior
        resetData();
    }
    // Check incoming data
    if (radio.available())
    {
        radio.read(&pot, sizeof(byte));
        lastReceiveTime = millis();
    }
    // Update Relay & Servo
    updateRelay();
    updateServo();
    // Debug
    Serial.println(pot);
}
