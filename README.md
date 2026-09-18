# 🔥 An IoT-Based Hybrid Fire-Fighting Robot with Real-Time Monitoring and Control System via a Web-Based Dashboard

An autonomous fire-fighting robot built using **dual ESP32 microcontrollers**, combining real-time environmental sensing, autonomous navigation, fire extinguishing, live video streaming, and a web-based monitoring and control system.

---
## 🚀 Overview

The system uses two ESP32 boards:

- **ESP32 DevKit** — sensor processing, motor control, fire detection, pump control, web dashboard, and autonomous navigation
- **ESP32-CAM** — live video streaming, pan-tilt camera control, and adjustable LED flash

The robot can autonomously detect the direction of a fire, navigate toward it, activate a water pump, and sweep the nozzle across a **45°–135°** range to extinguish the flame. A real-time web dashboard provides sensor monitoring and manual control.

---
## ✨ Key Features

- 🔥 **Directional Fire Detection**
  - 3 analog flame sensors for left, center, and right fire detection
  - Detects fire direction and adjusts robot movement accordingly

- 🤖 **Autonomous Navigation**
  - Automatically repositions toward the detected fire
  - Prioritizes fire detection and extinguishing logic
  - Activates the water pump when the robot reaches the target

- 💧 **Automatic Fire Extinguishing**
  - Motorized water pump
  - Servo-controlled sweeping nozzle
  - Nozzle operating range: **45°–135°**
  - Approximate sweep time: **2.16 seconds**

- 🌡️ **Environmental Monitoring**
  - MQ-2 gas sensor for smoke/gas detection
  - DHT11 for temperature and humidity monitoring
  - Real-time sensor data available through the web dashboard

- 🌐 **Real-Time Web Dashboard**
  - Built with HTML, CSS, and JavaScript
  - Autonomous and manual control modes
  - Motor control
  - Water pump control
  - Nozzle angle control
  - LED and buzzer control
  - Sensor monitoring with **500 ms polling interval**

- 📹 **Live Video Monitoring**
  - ESP32-CAM based live video streaming
  - Pan-tilt camera mechanism
  - Adjustable camera flash
  - WebSocket-based video communication
  - VGA resolution
  - Measured latency: **<200 ms**

- 📡 **Local Network Discovery**
  - Uses **mDNS** for zero-configuration device discovery
  - Allows the robot to be accessed through a local hostname instead of manually entering its IP address

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
