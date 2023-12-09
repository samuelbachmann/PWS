#include <SoftwareSerial.h>

// Define the pin numbers for RX and TX for the SoftwareSerial communication
#define RX_PIN 0 // Change this to your actual RX pin number
#define TX_PIN 1 // Change this to your actual TX pin number
#define GPS_BAUD 9600 // Replace 9600 with the actual baud rate of your GPS module


SoftwareSerial ss(RX_PIN, TX_PIN);


void setup() {
  Serial.begin(9600);
  ss.begin(GPS_BAUD);
}

void loop() {
  if (ss.available()) {
    char c = ss.read();
    Serial.print(c);
  }
}
