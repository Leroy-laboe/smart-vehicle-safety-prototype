# Smart Vehicle Safety Prototype

**An Arduino-based embedded safety prototype that monitors impact-level acceleration and rapid rotational motion using an MPU6050 IMU, then provides local visual and audible alerts.**

Built with **Arduino UNO, MPU6050, SSD1306 OLED, buzzer, LED, and Wokwi**.

[Open Wokwi Simulation](https://wokwi.com/projects/446325968538712065) · [GitHub Profile](https://github.com/Leroy-laboe) · [LinkedIn](https://www.linkedin.com/in/leroy-nyasha-mangwarara-86185a302/)

> **Prototype scope:** this project demonstrates threshold-based motion-event detection in simulation. It is not a certified automotive crash-detection or emergency-response system.

---

## Overview

The system continuously reads six-axis motion data from an **MPU6050**:

- 3-axis acceleration;
- 3-axis angular velocity.

The Arduino converts raw IMU values into:

- acceleration magnitude in **g**;
- gyroscope magnitude in **degrees per second**.

Two threshold rules are then evaluated:

- **impact event** — acceleration magnitude exceeds **2.5 g**;
- **rapid rotation event** — gyroscope magnitude exceeds **250°/s**.

When either condition is detected, the system activates:

- a red LED;
- an audible buzzer;
- an OLED warning message;
- Serial Monitor diagnostic output.

---

## System Flow

```text
        MPU6050
           │
           ▼
      Arduino UNO
           │
           ├── Convert raw accelerometer values
           ├── Convert raw gyroscope values
           │
           ▼
   Calculate vector magnitudes
           │
           ▼
      Threshold checks
           │
     ┌─────┴─────────┐
     ▼               ▼
Accel > 2.5 g    Gyro > 250°/s
     │               │
     └───────┬───────┘
             ▼
        Alert state
             │
      ┌──────┼──────┐
      ▼      ▼      ▼
     LED   Buzzer   OLED
```

The loop updates approximately every **500 ms**.

---

## Detection Logic

### Acceleration magnitude

The firmware converts the MPU6050 accelerometer readings using the default ±2 g sensitivity scale:

```text
ax = accX / 16384
ay = accY / 16384
az = accZ / 16384
```

It then calculates:

```text
accelMagnitude = √(ax² + ay² + az²)
```

An impact alert is triggered when:

```text
accelMagnitude > 2.5 g
```

### Angular-velocity magnitude

The gyroscope values are converted using the default ±250°/s scale factor:

```text
gx = gyroX / 131
gy = gyroY / 131
gz = gyroZ / 131
```

The firmware calculates:

```text
gyroMagnitude = √(gx² + gy² + gz²)
```

A rapid-rotation alert is triggered when:

```text
gyroMagnitude > 250°/s
```

> High rotational speed can indicate a severe orientation event, but this implementation does **not** estimate vehicle orientation or prove that a rollover occurred.

---

## Hardware / Simulation Components

| Component | Role |
| --- | --- |
| Arduino UNO | Main controller |
| MPU6050 | 3-axis accelerometer + 3-axis gyroscope |
| SSD1306 128×64 OLED | Live readings and alert status |
| Red LED + 220 Ω resistor | Visual alert |
| Buzzer | Audible alert |

---

## Pin Mapping

### I2C devices

Both the MPU6050 and SSD1306 share the Arduino UNO I2C bus.

| Signal | Arduino UNO |
| --- | --- |
| SDA | A4 |
| SCL | A5 |
| VCC | 5V |
| GND | GND |

### Alert outputs

| Component | Arduino UNO |
| --- | ---: |
| Buzzer | D7 |
| Red LED | D8 |

The OLED uses I2C address:

```text
0x3C
```

---

## Display States

### Normal

The OLED displays:

- acceleration magnitude;
- gyroscope magnitude;
- `Status: Normal`.

### Impact detected

```text
CRASH!
```

### Rapid rotation detected

```text
ROLLOVER!
```

### Both thresholds exceeded

```text
CRASH+ROLL!
```

The display labels are intentionally short to fit the 128×64 screen. The underlying logic should be interpreted as threshold-based impact and rapid-rotation detection.

---

## Serial Diagnostics

The firmware prints:

- acceleration magnitude;
- angular-velocity magnitude;
- approximate MPU6050 temperature;
- alert state.

Serial Monitor baud rate:

```text
9600
```

---

## Wokwi Simulation

The project can be opened directly in Wokwi:

**https://wokwi.com/projects/446325968538712065**

The cleaned circuit matches the firmware:

- buzzer → **D7**;
- LED → **D8** through **220 Ω**;
- MPU6050 + OLED → shared I2C bus.

---

## Required Libraries

- `MPU6050`
- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `Wire` (Arduino core)

---

## Repository Structure

```text
.
├── sketch.ino
├── diagram.json
├── libraries.txt
├── wokwi-project.txt
├── .gitignore
└── README.md
```

---

## What This Project Demonstrates

- embedded C++ / Arduino development;
- I2C communication;
- accelerometer and gyroscope processing;
- vector-magnitude calculations;
- threshold-based event detection;
- OLED user feedback;
- audible and visual alert control;
- sensor diagnostics;
- Wokwi-based embedded-system simulation.

---

## Limitations

This is an educational embedded prototype.

Current limitations include:

- fixed thresholds rather than calibrated vehicle-specific thresholds;
- acceleration magnitude includes gravity;
- no filtering or sensor-fusion algorithm;
- high angular velocity is treated as a rapid-rotation event rather than confirmed rollover;
- no GPS or location reporting;
- no GSM/SMS emergency notification;
- no persistent event logging;
- no automotive-grade hardware validation;
- no false-positive / false-negative evaluation against labelled crash data.

---

## Possible Extensions

- subtract gravity using orientation estimation;
- add complementary or Kalman filtering;
- calculate pitch and roll angles;
- require sustained or multi-sensor conditions before triggering;
- add GPS location;
- add GSM/SMS emergency alerts;
- log incidents to SD card or cloud storage;
- perform threshold calibration using labelled motion data;
- add reset / acknowledgement handling after an alert.

---

## Author

**Leroy Nyasha Mangwarara**

Computer Science · Software Engineering · Embedded Systems · IoT

[GitHub](https://github.com/Leroy-laboe) · [LinkedIn](https://www.linkedin.com/in/leroy-nyasha-mangwarara-86185a302/) · [Email](mailto:mangwararaleroy@gmail.com)
