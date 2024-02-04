#include <GY521.h>

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const byte MOVEMENT_SIZE = 3; //Increase this for more averaging. 4 is good.
byte movementRates[MOVEMENT_SIZE]; //Array of heart rates
byte movementSpot = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  mpu.initialize();

  Serial.println("Testing MPU-6050 connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t MARGIN_OF_ERROR = 700;


  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (abs(abs(ax) - movementRates[0]) < MARGIN_OF_ERROR || abs(abs(ay) - movementRates[1]) < MARGIN_OF_ERROR || abs(abs(az) - movementRates[2]) < MARGIN_OF_ERROR) {
    Serial.println("No Movement");
    movementRates[0] = abs(ax);
    movementRates[1] = abs(ay);
    movementRates[2] = abs(az);
  } else {
    Serial.println("Movement");
  }

  /*
  Serial.print("Acceleration: ");
  Serial.print("x = "); Serial.print(ax);
  Serial.print(" | y = "); Serial.print(ay);
  Serial.print(" | z = "); Serial.println(az);

  Serial.print("Rotation: ");
  Serial.print("x = "); Serial.print(gx);
  Serial.print(" | y = "); Serial.print(gy);
  Serial.print(" | z = "); Serial.println(gz);
  */

  delay(1000); // delay for readability
}