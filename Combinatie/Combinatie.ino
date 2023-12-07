#include <MQ135.h>
#include "TinyGPSpp.h"
#include <SoftwareSerial.h>

#define PIN_MQ135 A2
#define RX_PIN 0
#define TX_PIN 1
#define GPS_BAUD 9600
#define DELAY_INTERVAL 1000 // Changed delay to 1 second (1000 milliseconds)
#define GPS_PROCESS_THRESHOLD 10
#define GPS_WAIT_TIME 5000

MQ135 mq135_sensor(PIN_MQ135);
TinyGPSPlus gps;
SoftwareSerial ss(RX_PIN, TX_PIN);

const float TEMPERATURE = 21.0;
const float HUMIDITY = 25.0;

void setup() {
  Serial.begin(9600);
  ss.begin(GPS_BAUD);
}

void loop() {
  readGPS();
  readMQ135();
  delay(DELAY_INTERVAL); // Add a delay of 1 second between iterations
}

void readSensors() {
  readMQ135();
  readGPS();
}

void readMQ135() {
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(TEMPERATURE, HUMIDITY);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(TEMPERATURE, HUMIDITY);

  printMQ135Data(rzero, correctedRZero, resistance, ppm, correctedPPM);
}

void printMQ135Data(float rzero, float correctedRZero, float resistance, float ppm, float correctedPPM) {
  Serial.print("MQ135: RZero: ");
  Serial.print(rzero);
  Serial.print("  Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("  Resistance: ");
  Serial.print(resistance);
  Serial.print("  PPM: ");
  Serial.print(ppm);
  Serial.print("  Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println(" ppm");
}

void readGPS() {
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      printGPSData();
      return; // Exit the function after printing GPS data once
    }
  }

  if (millis() > GPS_WAIT_TIME && gps.charsProcessed() < GPS_PROCESS_THRESHOLD) {
    Serial.println(F("No GPS detected: check wiring."));
    // Handle GPS absence without blocking
  }
}

void printGPSData() {
  Serial.print("GPS: Location: ");
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(", ");
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print("INVALID");
  }

  Serial.print("  Date: ");
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.year());
  } else {
    Serial.print("INVALID");
  }

  Serial.println();
}