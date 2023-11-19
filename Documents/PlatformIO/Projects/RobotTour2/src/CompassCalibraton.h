
// int x_min = 1234123;
// int x_max = -1234123;
// int y_min = 1234123;
// int y_max = -1234123;
// int z_min = 1234123;
// int z_max = -1234123;

// void calibrate_compass() {
//   Serial.println("This will provide calibration settings for your QMC5883L chip. When prompted, move the magnetometer in all directions until the calibration is complete.");
//   Serial.println("Calibration will begin in 5 seconds.");
//   delay(5000);

//   Serial.println("CALIBRATING. Keep moving your sensor...");
//   compass.calibrate();

//   Serial.println("DONE. Copy the lines below and paste it into your projects sketch.");
//   Serial.println();
//   Serial.print("compass.setCalibrationOffsets(");
//   Serial.print(compass.getCalibrationOffset(0));
//   Serial.print(", ");
//   Serial.print(compass.getCalibrationOffset(1));
//   Serial.print(", ");
//   Serial.print(compass.getCalibrationOffset(2));
//   Serial.println(");");
//   Serial.print("compass.setCalibrationScales(");
//   Serial.print(compass.getCalibrationScale(0));
//   Serial.print(", ");
//   Serial.print(compass.getCalibrationScale(1));
//   Serial.print(", ");
//   Serial.print(compass.getCalibrationScale(2));
//   Serial.println(");");
// }



  //compass.setSmoothing(5, 1);
  //calibrate_compass();
  //compass.setCalibrationOffsets(-73.00, -749.00, -504.00);
  //compass.setCalibrationScales(1.05, 0.85, 1.14);




    // char buf[100];
  // compass.read();
  // sprintf(buf, "Compass: %d %d %d %d", compass.getX(), compass.getY(), compass.getZ(), compass.getAzimuth());
  // Serial.println(buf);
  // if (compass.getX() < x_min)
  // {
  //   x_min = compass.getX();
  // }
  // if (compass.getX() > x_max)
  // {
  //   x_max = compass.getX();
  // }
  // if (compass.getY() < y_min)
  // {
  //   y_min = compass.getY();
  // }
  // if (compass.getY() > y_max)
  // {
  //   y_max = compass.getY();
  // }
  // if (compass.getZ() < z_min)
  // {
  //   z_min = compass.getZ();
  // }
  // if (compass.getZ() > z_max)
  // {
  //   z_max = compass.getZ();
  // }
  // if (millis() >= 20000)
  // {
  //   Serial.print("X: ");
  //   Serial.print(x_min);
  //   Serial.print(" ");
  //   Serial.print(x_max);
  //   Serial.print(" ");
  //   Serial.print(" Y: ");
  //   Serial.print(y_min);
  //   Serial.print(" ");
  //   Serial.print(y_max);
  //   Serial.print(" ");
  //   Serial.print(" Z: ");
  //   Serial.print(z_min);
  //   Serial.print(" ");
  //   Serial.print(z_max);
  //   Serial.println();
  // }
