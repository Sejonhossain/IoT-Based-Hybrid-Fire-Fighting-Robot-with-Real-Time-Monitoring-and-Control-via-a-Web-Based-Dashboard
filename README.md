# 🔥 An IoT-Based Hybrid Fire-Fighting Robot with Real-Time Monitoring and Control System via a Web-Based Dashboard

An autonomous fire-fighting robot built using **dual ESP32 microcontrollers**, combining real-time environmental sensing, autonomous navigation, fire extinguishing, live video streaming, and a web-based monitoring and control system.

---

## 🚀 Overview

This project presents an autonomous fire-fighting robot designed to detect the direction of a fire, navigate toward it, and automatically extinguish it using a water pump and servo-controlled nozzle.

The system uses two ESP32 microcontrollers. The main ESP32 DevKit handles sensor processing, autonomous navigation, motor control, fire extinguishing, and the web dashboard. The ESP32-CAM provides live video streaming with pan-tilt camera control.

A real time web-based dashboard allows users to monitor sensor data and manually control the robot when required.

---

## ✨ Key Features

- 🔥 **Directional Fire Detection**
  - 3 analog flame sensors for left, center, and right fire detection
  - Determines the direction of the detected fire
  - Continuously monitors the surrounding environment

- 🤖 **Autonomous Navigation**
  - Automatically repositions the robot toward the detected fire
  - Uses directional flame sensor readings for navigation
  - Prioritizes fire detection and extinguishing logic
  - Automatically stops when the target position is reached

- 💧 **Automatic Fire Extinguishing**
  - Motorized water pump
  - Relay-controlled pump activation
  - Servo-controlled sweeping nozzle
  - Nozzle operating range: **45°–135°**
  - Approximate nozzle sweep time: **2.16 seconds**

- 🌡️ **Environmental Monitoring**
  - MQ-2 gas sensor for smoke/gas detection
  - DHT11 temperature and humidity sensor
  - Real-time environmental data displayed on the dashboard

- 🌐 **Real-Time Web Dashboard**
  - Built using HTML, CSS, and JavaScript
  - Autonomous and manual operating modes
  - Real-time robot control
  - Motor control
  - Water pump control
  - Nozzle angle control
  - LED control
  - Buzzer control
  - Sensor monitoring
  - Sensor data polling every **500 ms**

- 📹 **Live Video Monitoring**
  - ESP32-CAM based live video streaming
  - VGA resolution
  - WebSocket-based communication
  - Measured video latency of **<200 ms**
  - Real-time camera monitoring

- ↔️ **Pan-Tilt Camera System**
  - Servo-controlled horizontal camera movement
  - Servo-controlled vertical camera movement
  - Adjustable camera viewing direction

- 💡 **Camera Flash Control**
  - Adjustable ESP32-CAM flash
  - Flash can be controlled from the web dashboard

- 🎮 **Manual Override**
  - Manual robot movement through the web dashboard
  - Forward, backward, left, right, and stop controls
  - Manual pump and nozzle control
  - Allows the operator to override autonomous operation

- 📡 **Wireless Communication**
  - WiFi-based communication
  - HTTP communication for dashboard functions
  - WebSocket for real-time communication

- 🔎 **mDNS Device Discovery**
  - Uses mDNS for zero-configuration local network discovery
  - Allows the robot to be accessed using a hostname instead of manually entering its IP address

- 🚨 **Alert System**
  - LED-based status indication
  - Buzzer-based alerts
  - Fire detection status indication

---

## 🔧 Hardware Components

### Main Controller

- ESP32 DevKit

### Camera Controller

- ESP32-CAM (AI-Thinker)

### Sensors

- Analog Flame Sensor ×3
- MQ-2 Gas/Smoke Sensor
- DHT11 Temperature & Humidity Sensor

### Motor & Actuation

- L298N Motor Driver
- DC Gear Motors ×4
- Water Pump
- 1-Channel Relay Module
- Servo Motor for Fire-Fighting Nozzle
- Servo Motors for Camera Pan-Tilt

### Indicators

- LEDs
- Buzzer

### Mechanical Components

- Robot Chassis
- Wheels
- Pan-Tilt Camera Mechanism
- Water Tank
- Fire-Fighting Nozzle

---

## 💻 Software & Technologies

### Programming

- **C/C++**
- **Arduino Framework**

### Development Environment

- **Arduino IDE**
- **ESP32 Arduino Core**

### Web Technologies

- **HTML5**
- **CSS3**
- **JavaScript**

### Communication & Networking

- **WiFi**
- **HTTP**
- **WebSocket**
- **mDNS**

### ESP32 Libraries / Frameworks

- **ESPAsyncWebServer**
- **WebServer**
- **WebSocket**
- **WiFi**
- **ESP32 Camera Library**

---

## 🌐 Web Dashboard

The web dashboard provides a centralized interface for monitoring and controlling the fire-fighting robot.

### 📊 Real-Time Monitoring

The dashboard displays:

- 🌡️ Temperature
- 💧 Humidity
- 💨 Smoke/Gas status
- 🔥 Fire detection status
- 🎯 Fire direction
- 🤖 Robot operating mode
- 💧 Water pump status
- 🔄 Nozzle position
- 📹 Live camera feed

### 🎮 Robot Control

The dashboard provides controls for:

- ⬆️ Forward
- ⬇️ Backward
- ⬅️ Turn Left
- ➡️ Turn Right
- ⏹️ Stop
- 💧 Pump ON/OFF
- 🔄 Nozzle angle
- 💡 LED
- 🔔 Buzzer

### 🎥 Camera Control

- 📹 Live video feed
- ↔️ Pan control
- ↕️ Tilt control
- 💡 Flash control

### 🤖 Operating Modes

**Autonomous Mode**

The robot automatically detects and approaches the fire and activates the extinguishing system.

**Manual Mode**

The operator can manually control the robot, pump, nozzle, and camera through the web dashboard.

---

## 📊 Test Results

| Parameter | Result |
|---|---:|
| Autonomous navigation test cases | **5/5 successful** |
| Fire detection range | **20–30 cm** |
| Nozzle sweep time | **~2.16 s** |
| Dashboard sensor polling | **500 ms** |
| Video resolution | **VGA** |
| Video latency | **<200 ms** |

> **Note:** Test results are based on the project's experimental setup and conditions.

---

## 🏗️ System Architecture

The system consists of **two ESP32 boards**:

### 1. Main ESP32 DevKit

Responsible for:

- 🔥 Fire detection using three flame sensors
- 🌡️ Temperature and humidity monitoring using DHT11
- 💨 Smoke and gas monitoring using MQ-2
- 🚗 Motor control and autonomous navigation
- 💧 Water pump control
- 🔄 Fire-fighting nozzle control
- 🌐 Web dashboard hosting
- 🎮 Manual robot control
- 🚨 LED and buzzer control
- 📡 WiFi and mDNS communication

### 2. ESP32-CAM

Responsible for:

- 📹 Live video streaming
- ↔️ Camera pan control
- ↕️ Camera tilt control
- 💡 Flash control
- 📡 WebSocket-based video communication

---

## ⚙️ Working Principle

The three flame sensors continuously monitor the environment and determine the **direction of the fire** based on their sensor readings.

When a fire is detected, the main ESP32 processes the sensor data and determines whether the robot should move **left, right, forward, or stop** to approach the fire source.

After reaching the appropriate position, the robot activates the **water pump** through the relay module. The servo-controlled nozzle then sweeps across the fire area from **45° to 135°** to distribute water over the target.

At the same time, the MQ-2 and DHT11 sensors monitor **smoke/gas, temperature, and humidity**. These values are displayed on the web dashboard for real-time monitoring.

The ESP32-CAM independently provides a **live video stream**, while its pan-tilt mechanism and flash can be controlled through the web dashboard.

The system also supports **manual control**, allowing the operator to control the robot's movement, water pump, nozzle, and camera when required.

---

## 🚀 Future Work

- 🧠 **AI-Based Fire Detection & Classification**
  - Implement AI models for more accurate fire detection and classification

- 📹 **Computer Vision**
  - Use the ESP32-CAM for vision-based fire and environment analysis

- 🚧 **Obstacle Detection & Avoidance**
  - Integrate obstacle detection sensors for safer autonomous navigation

- 🤖 **Advanced Autonomous Navigation**
  - Improve navigation accuracy and develop more intelligent path-planning algorithms

- 💨 **Advanced Gas & Smoke Sensing**
  - Integrate additional sensors for improved gas and smoke detection

- 💧 **Adaptive Water Spraying**
  - Develop an adaptive spraying mechanism based on fire location and intensity

- ☁️ **Cloud-Based Monitoring**
  - Store and monitor sensor data through a cloud platform

- 📱 **Dedicated Mobile Application**
  - Develop a mobile application for remote monitoring and robot control

- 🔔 **Mobile Notifications**
  - Implement real-time push notifications for fire and system alerts

- 📊 **Historical Data & Analytics**
  - Store sensor data and provide historical visualization and analysis

- 🔋 **Improved Power Management**
  - Implement battery-level monitoring and optimize overall power consumption

- 🗺️ **GPS-Based Outdoor Navigation**
  - Enable GPS-assisted navigation for outdoor fire-fighting applications

- 🎥 **Improved Video Streaming**
  - Support higher-resolution and more efficient real-time video streaming

- 🌐 **Internet-Based Remote Access**
  - Enable secure remote monitoring and control over the Internet
    
---

###⭐ If you find this project useful or interesting, consider giving the repository a star!

---


