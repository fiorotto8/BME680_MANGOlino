#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

//Function to software reset
void(* Reset)(void) = 0;

// Initialization of BME680 sensor
Adafruit_BME680 bme; // I2C
int incomingByte = 0; // Variable to store incoming serial data

void setup(){
  Serial.begin(115200); // Start serial communication at 115200 baud rate

  // Initialize BME680 sensor
  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    //while (1); // Uncomment to halt if sensor is not found
  }
  else{
    //Serial.println(F("BME initialized!"));
  }

  // Set up oversampling and filter initialization for BME680
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // Set heater temperature and duration
}

// Main loop function, runs repeatedly
void loop() {
    // Check if data is available on the serial port
    if (Serial.available() > 0) {
        incomingByte = Serial.read(); // Read the incoming byte

      // Uncomment for degub about message received
      //Serial.print("Command received is: ");
      //Serial.println(incomingByte);
      //delay(100);
      
      // Handle BME sensor reading
      if (incomingByte == 82) { // 'R' command
        unsigned long endTime = bme.beginReading();
        if (endTime == 0) {
          Serial.println(F("Failed to begin reading :("));
          return;
        }
        if (!bme.endReading()) {
          Serial.println(F("Failed to complete reading :("));
          return;
        }
        // Output sensor readings
        Serial.print(bme.temperature + 273.15); // Convert to Kelvin
        Serial.print(";");
        Serial.print(bme.pressure);
        Serial.print(";");
        Serial.print(bme.humidity);
        Serial.print(";");
        Serial.println(bme.gas_resistance);
      }

      // Identify device
      if (incomingByte == 87) { // 'W' command
        Serial.println("MANGOlino");
      }

          //Software reset the board
      if (incomingByte == 75) { // 'K' command
        Serial.println("Resetting Arduinio...");
        delay(100);
        Reset();
      }
    }
    delay(100);
}