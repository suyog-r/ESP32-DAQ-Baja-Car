# ESP32-Based Vehicle Data Acquisition System (Baja Car)

## Overview
Designed and implemented a real-time multi-sensor Data Acquisition System (DAQ) for an M-Baja car using ESP32. The system monitors critical performance parameters such as acceleration, RPM, temperature, and speed, and logs data to the cloud for analysis.

> Note: This repository contains a structured version of the firmware used during real-world testing. Some modules are simplified for clarity.

---

## System Architecture

ESP32 acts as the central controller interfacing with multiple sensors:

- MPU6050 → I2C → Acceleration (linear & lateral)
- MLX90614 → I2C → Temperature (disc & CVT)
- GPS NEO-7M → UART → Speed measurement
- LM393 → Interrupt → RPM measurement
- Laser Gates → ADC → Acceleration validation

All sensor data is processed and optionally transmitted to Firebase for real-time monitoring.

---

## Key Features

- Multi-sensor real-time data acquisition system
- Lateral and linear acceleration measurement using MPU6050
- Acceleration validation using dual laser gate timing system
- Interrupt-based RPM measurement using LM393 sensor
- Temperature monitoring of disc brake and CVT using MLX90614
- Speed measurement using GPS module
- Noise reduction using filtering techniques
- Sensor calibration for improved accuracy

---

## Hardware Used

- ESP32 Microcontroller
- MPU6050 (Accelerometer + Gyroscope)
- LM393 RPM Sensor
- MLX90614 IR Temperature Sensor
- GPS NEO-7M Module
- RTC DS3231 (for timestamping)
- Laser + LDR setup (for timing-based measurement)

---

## Firmware Concepts Used

- Embedded C (ESP32)
- I2C communication (MPU6050, MLX90614)
- UART communication (GPS module)
- Interrupt handling (RPM measurement)
- ADC-based sensing (Laser gates using LDR)
- Real-time data acquisition and processing
- Signal filtering (low-pass filtering)
- Sensor calibration techniques

---

## Results

- Maximum RPM recorded: **4000 RPM**
- Maximum speed recorded: **60 km/h**
- Acceleration validated: **3.2 m/s²**
- Temperature observed up to: **90°C**

---

## Challenges Faced

- Sensor noise due to vehicle vibrations
- Calibration of accelerometer for accurate readings
- Synchronization of multiple sensors in real-time
- Reliable interrupt handling for RPM measurement
- GPS signal instability in certain conditions

---

## Learnings

- Designing multi-sensor embedded systems
- Real-time data acquisition and processing
- Interrupt-driven programming in ESP32
- Debugging hardware-software integration issues
- Working with multiple communication protocols (I2C, UART)
- Applying filtering and calibration for accurate measurements

---

## Future Improvements

- Integration of all modules into a unified firmware architecture
- Data visualization dashboard for real-time monitoring
- Improved sensor fusion using Kalman filtering
- Power optimization for long-duration operation
