#include <MQ135.h>
#include "TinyGPSpp.h"
#include <SoftwareSerial.h>
#include <SdFat.h>

#define PIN_MQ135 A2
#define RX_PIN 0
#define TX_PIN 1
#define GPS_BAUD 9600
#define DELAY_INTERVAL 1000
#define GPS_PROCESS_THRESHOLD 10
#define GPS_WAIT_TIME 5000

SdFat SD;
SdFile dataFile;

MQ135 mq135_sensor(PIN_MQ135);
TinyGPSPlus gps;
SoftwareSerial ss(RX_PIN, TX_PIN);

const float TEMPERATURE = 21.0;
const float HUMIDITY = 25.0;

void setup() {
  Serial.begin(9600);
  ss.begin(GPS_BAUD);

  if (!SD.begin(10, SPI_HALF_SPEED)) {
    Serial.println(F("SD initialization failed!"));
    return;
  }

  if (!SD.exists("sensor_data.txt")) {
    dataFile.open("sensor_data.txt", O_RDWR | O_CREAT | O_AT_END);
    if (dataFile) {
      dataFile.println("Starting Sensor Data Log:");
      dataFile.close();
    } else {
      Serial.println(F("Error creating data file!"));
      return;
    }
  }

  dataFile.open("sensor_data.txt", O_RDWR | O_CREAT | O_AT_END);
  if (!dataFile) {
    Serial.println(F("Error opening data file!"));
    return;
  }
}

void loop() {
  readSensors();
  delay(DELAY_INTERVAL);
}

void readSensors() {
  readMQ135();
  readGPS();
  writeDataToFile();
}

void readMQ135() {
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(TEMPERATURE, HUMIDITY);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(TEMPERATURE, HUMIDITY);

  printMQ135Data(rzero, correctedRZero, resistance, ppm, correctedPPM);
}

void readGPS() {
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      printGPSData();
      return;
    }
  }

  if (millis() > GPS_WAIT_TIME && gps.charsProcessed() < GPS_PROCESS_THRESHOLD) {
    Serial.println(F("No GPS detected: check wiring."));
  }
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

void writeDataToFile() {
  if (dataFile) {
    float rzero = mq135_sensor.getRZero();
    float correctedRZero = mq135_sensor.getCorrectedRZero(TEMPERATURE, HUMIDITY);
    float resistance = mq135_sensor.getResistance();
    float ppm = mq135_sensor.getPPM();
    float correctedPPM = mq135_sensor.getCorrectedPPM(TEMPERATURE, HUMIDITY);

    dataFile.print("MQ135: ");
    dataFile.print("RZero: ");
    dataFile.print(rzero);
    dataFile.print("  Corrected RZero: ");
    dataFile.print(correctedRZero);
    dataFile.print("  Resistance: ");
    dataFile.print(resistance);
    dataFile.print("  PPM: ");
    dataFile.print(ppm);
    dataFile.print("  Corrected PPM: ");
    dataFile.print(correctedPPM);
    dataFile.println(" ppm");

    dataFile.print("GPS: ");
    if (gps.location.isValid()) {
      dataFile.print("Location: ");
      dataFile.print(gps.location.lat(), 6);
      dataFile.print(", ");
      dataFile.print(gps.location.lng(), 6);
    } else {
      dataFile.print("Location: INVALID");
    }

    if (gps.date.isValid()) {
      dataFile.print("  Date: ");
      dataFile.print(gps.date.month());
      dataFile.print("/");
      dataFile.print(gps.date.day());
      dataFile.print("/");
      dataFile.print(gps.date.year());
    } else {
      dataFile.print("  Date: INVALID");
    }

    dataFile.println();
  } else {
    Serial.println(F("Error writing to the file!"));
  }
}
