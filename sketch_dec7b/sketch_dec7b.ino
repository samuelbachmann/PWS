#include <SdFat.h>
#include <SPI.h>

const int SD_CS_PIN = 10; // Use pin 10 for built-in SD card

SdFat SD;
SdFile myFile;

void setup() {
  Serial.begin(9600);
  
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD initialization failed!");
    return;
  }

  if (!myFile.open("example.txt", O_RDWR | O_CREAT)) {
    Serial.println("Error opening file!");
  } else {
    Serial.println("File opened successfully.");
    myFile.println("Hello, SD card!");
    myFile.close();
    Serial.println("Data written to file.");
  }
}

void loop() {
  // Nothing in the loop for this example
}
