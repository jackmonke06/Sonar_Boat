void loop() {

  // Get GPS readings
  latitude = gps.location.lat();
  longitude = gps.location.lng();

  // Get sonar reading
  depth = sonarDepth;

  // Only save if GPS has a valid fix
  if (gps.location.isValid()) {

    File file = SD.open("/sonar_data.csv", FILE_APPEND);

    if (file) {
      file.print(longitude, 6);
      file.print(",");
      file.print(latitude, 6);
      file.print(",");
      file.println(depth, 2);

      file.close();
    }
  }
}