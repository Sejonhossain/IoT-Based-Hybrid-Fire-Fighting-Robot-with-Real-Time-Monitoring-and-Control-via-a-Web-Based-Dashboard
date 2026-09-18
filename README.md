# 🔥 IoT-Based Fire-Fighting Robot

An IoT-based hybrid fire-fighting robot with **autonomous fire detection, real-time monitoring, and web-based remote control**.

---

## 📌 Overview

This project presents a **WiFi-controlled fire-fighting robot** designed to detect and extinguish fire while providing real-time monitoring through a web-based dashboard.

The system uses **two ESP32 microcontrollers**. The main ESP32 controls the sensors, motors, water pump, and web dashboard, while the ESP32-CAM provides **live video streaming with pan-tilt camera control**.

---

## ✨ Features

- 🔥 Autonomous fire detection
- 🎯 Directional fire detection using three flame sensors
- 💨 Smoke detection using MQ2 sensor
- 🌡️ Temperature and humidity monitoring using DHT11
- 🚒 Automatic fire extinguishing using water pump
- 🔄 Servo-controlled nozzle
- 📹 ESP32-CAM live video streaming
- ↔️ Pan and tilt camera control
- 🎮 Manual robot control
- 🌐 Web-based dashboard
- 📡 WiFi communication
- 🔎 mDNS-based device discovery

---

## 🔧 Hardware Components

- ESP32
- ESP32-CAM (AI-Thinker)
- Flame Sensors ×3
- MQ2 Gas Sensor
- DHT11 Temperature & Humidity Sensor
- L298N Motor Driver
- DC Motors
- Water Pump
- Relay Module
- Servo Motors
- LEDs
- Buzzer
- Robot Chassis

---

## 💻 Software & Technologies

- Arduino IDE
- ESP32 Arduino Core
- C/C++
- HTML
- CSS
- JavaScript
- WebServer
- WebSocket
- mDNS

---

## 🏗️ System Architecture

The system consists of **two ESP32 boards**:

### 1. Main ESP32

Responsible for:

- Fire detection
- Sensor monitoring
- Motor control
- Water pump control
- Web dashboard

### 2. ESP32-CAM

Responsible for:

- Live video streaming
- Pan/tilt control
- Flash control

---

## ⚙️ Working Principle

The flame sensors detect the direction of the fire. Based on the sensor readings, the robot automatically moves toward the fire source.

After reaching the appropriate position, the water pump is activated and the servo-controlled nozzle sweeps across the fire area.

The system also provides a **web dashboard** for real-time monitoring and manual control.


## 🚀 Future Work

- AI-based fire classification
- Computer vision using ESP32-CAM
- Advanced gas sensing
- Obstacle detection
- Autonomous navigation
- Cloud-based monitoring
- Mobile notifications
- Improved power management
- Adaptive water spraying mechanism

---
