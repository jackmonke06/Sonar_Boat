
#include <TinyGPSPlus.h>

// Create GPS parser
TinyGPSPlus gps;

// ESP32 hardware UART
HardwareSerial GPS(1);

// GPS pins
const int GPS_RX = 16;
const int GPS_TX = 17;

unsigned long lastPrint = 0;

void setup() {

  Serial.begin(115200);

  // Start GPS communication
  GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println("GPS Starting...");
}

void loop() {

  // Feed incoming GPS data into TinyGPSPlus
  while (GPS.available()) {
    gps.encode(GPS.read());
  }

  // Print information every second
  if (millis() - lastPrint >= 1000) {

    lastPrint = millis();

    Serial.println("----------------------");

    if (gps.location.isValid()) {

      Serial.println("GPS FIX: YES");

      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);

    } else {
      Serial.println("GPS FIX: NO");
    }

    Serial.print("Satellites: ");
    if (gps.satellites.isValid()) {
      Serial.println(gps.satellites.value());
    } else {
      Serial.println("Waiting...");
    }

    Serial.print("Altitude: ");
    if (gps.altitude.isValid()) {
      Serial.print(gps.altitude.meters());
      Serial.println(" m");
    } else {
      Serial.println("Waiting...");
    }

    Serial.print("Speed: ");
    if (gps.speed.isValid()) {
      Serial.print(gps.speed.knots());
      Serial.println(" knots");
    } else {
      Serial.println("Waiting...");
    }

    Serial.println("----------------------");
  }
}
