# Wireless IoT Air Quality Monitoring Node (LoRa/Zigbee Enabled)

## Overview
This project implements a wireless air quality monitoring system that collects environmental data and transmits it over LoRa/Zigbee for remote monitoring. The system measures particulate matter (PM1.0, PM2.5, PM10), temperature, and humidity, and is designed for distributed sensing applications such as UAV-based and field deployments.

## Key Features
- PMS5003 air quality sensing (PM1.0, PM2.5, PM10)
- Temperature & humidity sensing using AHT10
- Wireless communication via LoRa / Zigbee (UART-based)
- Custom serial protocol implementation for sensor data acquisition
- Real-time environmental data transmission
- Suitable for UAV-based and remote sensing applications

## System Architecture
(Add your architecture diagram here)

## Communication Layer
The system transmits processed sensor data using LoRa or Zigbee modules through UART communication. This enables reliable deployment in remote environments where real-time data needs to be sent to a ground station or central server.

## Key Engineering Contributions
- Implemented custom command-based communication for PMS5003 sensor
- Designed embedded data acquisition system using Arduino
- Integrated multiple sensors into a unified monitoring platform
- Developed wireless data transmission using LoRa/Zigbee
- Implemented checksum validation for reliable data handling

## Results
- Successfully measured PM1.0, PM2.5, PM10 in real-time
- Reliable wireless communication achieved using LoRa/Zigbee
- System tested in field-like conditions

## Future Improvements
- Cloud integration (AWS / ThingsBoard)
- Data visualization dashboard
- Power optimization for long-term deployment

## Note
This repository contains a simplified and non-confidential representation of work carried out during my time at CSIR–NEERI.
