#include <MQ135.h>
#include "TinyGPSpp.h"
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

#define PIN_MQ135 A2
#define RX_PIN 0
#define TX_PIN 1
#define GPS_BAUD 9600
#define DELAY_INTERVAL 1000
#define GPS_PROCESS_THRESHOLD 10
#define GPS_WAIT_TIME 5000

MQ135 mq135_sensor(PIN_MQ135);
TinyGPSPlus gps;
SoftwareSerial ss(RX_PIN, TX_PIN);

Sd2Card card;
File myFile;

const int chipSelect = BUILTIN_SDCARD;
const int ledPin = 13;
const float TEMPERATURE = 21.0;
const float HUMIDITY = 25.0;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    digitalWrite(ledPin, HIGH);
    return;
  }

  if (SD.remove("data.txt")) {
    Serial.println("Previous data file removed.");
  }

  ss.begin(GPS_BAUD);
  myFile = SD.open("data.txt", FILE_WRITE);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= DELAY_INTERVAL) {
    previousMillis = currentMillis;

    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);

    readMQ135();
    readGPS();;
  }
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
  myFile.print("MQ135: RZero: ");
  myFile.print(rzero);
  myFile.print("  Corrected RZero: ");
  myFile.print(correctedRZero);
  myFile.print("  Resistance: ");
  myFile.print(resistance);
  myFile.print("  PPM: ");
  myFile.print(ppm);
  myFile.print("  Corrected PPM: ");
  myFile.print(correctedPPM);
  myFile.println(" ppm");
}

void readGPS() {
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      printGPSData();
      return; // Exit the function after printing GPS data once
    }
  }

  if (millis() > GPS_WAIT_TIME && gps.charsProcessed() < GPS_PROCESS_THRESHOLD) {
    myFile.println(F("No GPS detected: check wiring."));
    // Handle GPS absence without blocking
  }
}

void printGPSData() {
  myFile.print("GPS: Location: ");
  if (gps.location.isValid()) {
    myFile.print(gps.location.lat(), 6);
    myFile.print(", ");
    myFile.print(gps.location.lng(), 6);
  } else {
    myFile.print("INVALID");
  }

  myFile.print("  Date: ");
  if (gps.date.isValid()) {
    myFile.print(gps.date.month());
    myFile.print("/");
    myFile.print(gps.date.day());
    myFile.print("/");
    myFile.print(gps.date.year());
  } else {
    myFile.print("INVALID");
  }

  myFile.println();
}