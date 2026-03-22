/*
 * Project: Wireless UAV Air Quality Monitoring System (LoRa/Zigbee)
 * Author: Pranav Agrawal
 *
 * Description:
 * Embedded system for real-time environmental monitoring using PMS5003
 * (PM1.0, PM2.5, PM10) and AHT10 (temperature, humidity).
 * Data is transmitted via LoRa/Zigbee modules.
 *
 * License: MIT License
 *
 * Copyright (c) 2026 Pranav Agrawal
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files, to deal in the Software
 * without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
 */

#include <Adafruit_AHTX0.h>
#include <SoftwareSerial.h>

// -------------------- Pin Configuration --------------------
#define PMS_RX_PIN 2
#define PMS_TX_PIN 3

// -------------------- Serial Setup --------------------
SoftwareSerial pmsSerial(PMS_RX_PIN, PMS_TX_PIN);
Adafruit_AHTX0 aht;

// -------------------- PMS5003 Data Structure --------------------
struct PMSData {
  uint16_t frameLength;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um;
  uint16_t particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

// -------------------- Command Definitions --------------------
enum CommandType {
  CMD_READ_MODE = 0x0A,
  CMD_READ_DATA = 0x05
};

// -------------------- AQI Sensor Class --------------------
class AQISensor {
public:
  PMSData data;

  AQISensor(Stream* stream) : serial(stream) {}

  void init() {
    sendCommand(CMD_READ_MODE);
  }

  bool readData() {
    sendCommand(CMD_READ_DATA);

    if (!serial->available()) {
      return false;
    }

    // Sync to frame start
    if (serial->peek() != 0x42) {
      serial->read();
      return false;
    }

    if (serial->available() < 32) {
      return false;
    }

    uint8_t buffer[32];
    serial->readBytes(buffer, 32);

    uint16_t sum = 0;
    for (int i = 0; i < 30; i++) {
      sum += buffer[i];
    }

    uint16_t parsed[15];
    for (int i = 0; i < 15; i++) {
      parsed[i] = (buffer[2 + i * 2] << 8) | buffer[2 + i * 2 + 1];
    }

    memcpy(&data, parsed, 30);

    if (sum != data.checksum) {
      Serial.println("Checksum error");
      return false;
    }

    return true;
  }

private:
  Stream* serial;

  void sendCommand(uint8_t cmd) {
    uint8_t frame[9] = {0xAA, cmd, 0, 0, 0, 0, 0, 0, 0xBB};

    uint16_t checksum = frame[0] + frame[1] + frame[2] +
                        frame[3] + frame[4] + frame[5] + frame[8];

    frame[6] = (checksum >> 8) & 0xFF;
    frame[7] = checksum & 0xFF;

    serial->write(frame, sizeof(frame));
  }
};

// -------------------- Global Objects --------------------
AQISensor aqi(&pmsSerial);

// -------------------- Setup --------------------
void setup() {
  Serial.begin(9600);
  pmsSerial.begin(9600);

  if (!aht.begin()) {
    Serial.println("AHT10 initialization failed");
    while (1);
  }

  aqi.init();

  Serial.println("PM1.0,PM2.5,PM10,Temp,Humidity");
}

// -------------------- Loop --------------------
void loop() {
  if (aqi.readData()) {

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    Serial.print(aqi.data.pm10_env);
    Serial.print(",");
    Serial.print(aqi.data.pm25_env);
    Serial.print(",");
    Serial.print(aqi.data.pm100_env);
    Serial.print(",");
    Serial.print(temp.temperature);
    Serial.print(",");
    Serial.print(humidity.relative_humidity);
    Serial.println();
  }

  delay(1000);
}
