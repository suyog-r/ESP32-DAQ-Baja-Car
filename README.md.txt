# ESP32-Based Vehicle Data Acquisition System (Baja Car)

## Overview
Developed a real-time data acquisition system for an M-Baja car using ESP32 to monitor key performance parameters such as acceleration, RPM, temperature, and speed.

## Note: This is a structured version of the firmware used during real-world testing. Some modules are simplified for clarity.

## Key Features
- Lateral acceleration measurement using MPU6050
- Acceleration validation using laser gate system
- RPM measurement using LM393 with interrupt-based counting
- Disc and CVT temperature monitoring using MLX90614
- Speed measurement using GPS NEO-7M

## Hardware Used
- ESP32 microcontroller
- MPU6050 (Accelerometer + Gyroscope)
- LM393 RPM sensor
- MLX90614 IR temperature sensor
- GPS NEO-7M module
- RTC DS3231
- Laser + LDR setup

## Firmware Concepts Used
- I2C communication (MPU6050, MLX90614)
- UART communication (GPS)
- Interrupts (RPM measurement)
- ADC (LDR sensor)
- Real-time data processing

## Results
- RPM measured up to 4000 RPM
- Max speed recorded: 60 km/h
- Acceleration validated: 3.2 m/s²
- Temperature monitored up to 90°C

## Challenges Faced
- Sensor noise and filtering
- Synchronization of multiple sensors
- Interrupt handling for accurate RPM
- Real-time data acquisition

## Learnings
- Embedded system integration
- Sensor interfacing
- Debugging real hardware systems
- Working with multiple communication protocols