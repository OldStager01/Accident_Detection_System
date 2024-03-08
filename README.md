# Accident Detection System

The Accident Detection System is a vehicle safety project designed to detect collisions using ESP, accelerometer, GPS, and GSM technologies. This system provides real-time alerts to emergency services and predefined contacts, enhancing road safety.

## Features

- **Collision Detection:** Utilizes accelerometer data to identify sudden changes in motion.
- **Real-time Location Tracking:** GPS sensor provides accurate location information.
- **Immediate Alerts:** GSM module sends instant text alerts with location data to emergency services and predefined contacts.
- **ESP Microcontroller:** Ensures seamless communication and system integration.

## Usage

1. **Hardware Requirements:** ESP32 microcontroller, accelerometer (ADXL335), GPS sensor (Neo6m), and GSM module (SIM900A). (Refer Circuit Diagram)
2. **Setup:**
   - Connect hardware components following the provided schematics.
   - Flash the provided code onto the ESP microcontroller using the Arduino IDE.
3. **Configuration:**
   - Update predefined contacts in the code.
   - Adjust sensitivity settings for collision detection.
