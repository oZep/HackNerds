#include <Wire.h>
#include "MAX30105.h"
#include <MPU6050.h>
#include <GY521.h>

#include "heartRate.h"


MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

const int buttonPin = 3;  // the number of the pushbutton pin
const int ledPin = 10;    // the number of the LED pin


MPU6050 mpu;

const byte MOVEMENT_SIZE = 3; //Increase this for more averaging. 4 is good.
byte movementRates[MOVEMENT_SIZE]; //Array of heart rates
byte movementSpot = 0;
const byte SET_RATE = 60;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");
  Wire.begin();
  mpu.initialize();
  
  Serial.println("Testing MPU-6050 connections...");
  Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

}

void loop()
{
  long irValue = particleSensor.getIR();
  
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t MARGIN_OF_ERROR = 700;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  digitalWrite(ledPin, LOW);
  
  if ((abs(abs(ax) - movementRates[0]) < MARGIN_OF_ERROR || abs(abs(ay) - movementRates[1]) < MARGIN_OF_ERROR || abs(abs(az) - movementRates[2]) < MARGIN_OF_ERROR) && beatsPerMinute < SET_RATE) {
    Serial.println("No Movement");
    digitalWrite(ledPin, HIGH);
    movementRates[0] = abs(ax);
    movementRates[1] = abs(ay);
    movementRates[2] = abs(az);
  } else {
    Serial.println("Movement");
  }



  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
  delay(1000);
}
message.txt
5 KB