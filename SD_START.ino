#include <SPI.h>
#include <SD.h>

// SD card chip select pin
#define SD_CS 5


// Example sensor variables
double latitude = 38.95420;
double longitude = -95.23510;
float depth = 2.45;

void setup() {

  Serial.begin(115200);

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  Serial.println("SD card initialized!");

  // Create file and write headers if it doesn't exist
  if (!SD.exists("/sonar_data.csv")) {

    File file = SD.open("/sonar_data.csv", FILE_WRITE);

    if (file) {
      file.println("Longitude,Latitude,Depth");
      file.close();
      Serial.println("CSV file created");
    }
  }
}

void loop() {

  // Replace these example values with actual GPS and sonar readings

  latitude = 38.95420;
  longitude = -95.23510;
  depth = 2.45;

  // Save measurements
  File file = SD.open("/sonar_data.csv", FILE_APPEND);

  if (file) {

    file.print(longitude, 6);
    file.print(",");
    file.print(latitude, 6);
    file.print(",");
    file.println(depth, 2);

    file.close();

    Serial.println("Data saved!");

  } else {

    Serial.println("Error opening file");

  }

  delay(1000);
}