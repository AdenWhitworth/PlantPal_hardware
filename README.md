<img width="80" src="https://github.com/AdenWhitworth/PlantPal_Front-End/raw/master/src/Images/PlantPal%20Logo.svg" alt="PlantPal Logo">

# PlantPal Hardware

Welcome to the **PlantPal Hardware**! This repository contains the ESP32 code, PCB schematic, and 3D models for the PlantPal device, designed to help plant enthusiasts monitor and manage their plants through a seamless and interactive experience.

## Table of Contents
- [Overview](#overview)
- [PlantPal Live Demo](#plantpal-live-demo)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Setting Up AWS IoT Core](#setting-up-aws-iot-core)
  - [Installation](#installation)
  - [Environment Variables](#environment-variables)
  - [Usage](#usage)
- [Documentation](#documentation)
- [Future Features](#future-features)
- [Contributing](#contributing)
- [License](#license)

## Overview

PlantPal is a smart gardening solution designed to help plant enthusiasts monitor and manage their house plants effectively. The hardware component integrates various sensors and actuators to automate plant care, including soil moisture monitoring and watering.

## PlantPal Live Demo

The PlantPal application is live and can be accessed at [PlantPal Demo](https://www.plantpalhome.com/). Watch the video below showing the PlantPal hardware in action! The PlantPal hardware, accompanied by the live application, offers an intuitive experience for managing your plants remotely.

### Test User Credentials

You can explore the full functionality of the PlantPal application using the following demo account:

- **Email:** support@plantpalhome.com
- **Password:** testpassword123

Please note that this test account is connected to an actual PlantPal device in my kitchen, so any interactions may affect its operation. Feel free to explore, but please be considerate of its usage!

### Getting a PlantPal Device

PlantPal devices will soon be available for sale on our website, which will provide the monitoring device and pumping extension. If you can't wait for PlantPal to be available on our website, feel free to 3D print your own and order your own PCB based on the CAD files included in this repository. 

## Features

- **Soil Moisture Sensing**: Monitor soil moisture levels to ensure optimal watering.
- **Automated Watering**: Control a water pump to deliver the right amount of water based on soil moisture readings.
- **Bluetooth Connectivity**: Manage the device's Wi-Fi through Bluetooth Low Energy (BLE) with our PlantPal application.
- **User-Friendly Interface**: Manage the PlantPal device settings through a button on the device.
- **MQTT Connectivity**: View your plants' information in real-time and interact with PlantPal from anywhere.
- **LED Health Indications**: Onboard RGB LED color-coded to let users see what is physically happening at all times.

## Technologies Used

- **C++**: A powerful object-oriented programming language used for developing the firmware of the PlantPal hardware. It provides the necessary features for efficient memory management and system-level programming.
- **C**: A foundational programming language utilized for low-level hardware control and interfacing. It allows direct manipulation of memory and hardware resources, which is essential for embedded systems development.
- **Arduino**: An open-source electronics platform based on easy-to-use hardware and software. The Arduino IDE is used to write and upload code to the ESP32 microcontroller, enabling rapid prototyping and development.
- **MQTT**: A lightweight messaging protocol designed for low-bandwidth, high-latency, or unreliable networks. MQTT is used for communication between the PlantPal hardware and AWS IoT Core, allowing for efficient data exchange and remote monitoring.
- **BLE (Bluetooth Low Energy)**: A wireless technology designed for short-range communication with low power consumption. BLE is utilized for connecting the PlantPal hardware to mobile devices for real-time monitoring and control.
- **AWS IoT Core**: A cloud service that enables secure device connectivity and management. AWS IoT Core is used for managing data sent from the PlantPal hardware, facilitating remote access and control through the cloud.
- **Doxygen**: A documentation generation tool that allows developers to create comprehensive documentation for their codebase. Doxygen is used to generate user-friendly documentation for the PlantPal hardware project, making it easier for other developers to understand and contribute to the project.

## Hardware Components

The PlantPal hardware setup includes the following components:

- **ESP32 Microcontroller**: The core component that manages all hardware interactions.
- **Intel ATSAMD10 Microcontroller**: Built-in capacitive touch measurement system that provides readings ranging from about 200 (very dry) to 2000 (very wet).
- **Water Pump**: Delivers water to the plant as needed using a 3V DC vertical water pump with PVC tubing.
- **LED Indicators**: Provides visual feedback on system status using an RGB LED.
- **Buttons**: Allow manual control and configuration of the system with a tactile switch.
- **Resistors**: Regulate current to the hardware accessories.
- **TIP Transistors**: Control the pump using a TIP120 transistor for power switching.
- **Diodes**: Ensure electronic safety by regulating current direction.
- **Capacitors**: Collect and store current for PCB reliability.

## Getting Started

Follow the instructions below to set up the project on your local machine.

### Prerequisites

Before you begin, ensure you have the following:

- [Arduino IDE](https://www.arduino.cc/en/software) installed.
- PlatformIO (if preferred) set up in your IDE.
- Necessary hardware components.
- AWS IoT Core Account:
  - Sign up for an AWS account if you don't have one.
  - Create a new IoT Thing in the AWS IoT Core console.
  - Attach a policy to allow publishing and subscribing to MQTT topics.
  - Create a certificate for the Thing and download the keys.

### Setting Up AWS IoT Core

1. Create a New Thing:
  - Go to the AWS IoT Core console.
  - Click on “Manage” -> “Things” -> “Create a Thing”.
  - Choose "Create a single thing" and provide a name for your Thing.
2. Create a Policy:
  - Go to "Secure" -> "Policies" -> "Create a policy".
  - Set up a policy that allows actions like iot:Publish, iot:Subscribe, and iot:Connect on your MQTT topics.
3. Attach the Policy to the Thing:
  - After creating the Thing, navigate back to it and select "Security" -> "Certificates".
  - Attach the certificate you created earlier and attach the policy.
4. Add MQTT Topics:
  - Ensure the following topics are set up for your application:
    - your_aws_iot_log_topic for publishing sensor logs.
    - $aws/things/{your_thing}/shadow/update for shadow updates.
    - $aws/things/{your_thing}/shadow/get for retrieving shadow states.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/AdenWhitworth/PlantPal_Front-End.git
   cd PlantPal_hardware
   ```
2. Open the project in the Arduino IDE or your preferred IDE with PlatformIO.
3. Install the required libraries:
  - ArduinoJson
  - MQTT
  - Adafruit seesaw Library
4. Order the PCB from your preferred PCB manufacturer.
5. Connect the pump by plugging in the 2p JST connector.
6. Upload the code to the ESP32.

### Environment Variables
Create a `utilities.h` and `utilities.cpp` file in the root directory and define the following variables:
  
  `utilities.h` **variables**:
  ```plain text
  // AWS IoT Core MQTT Topics
  #define AWS_IOT_PUBLISH_TOPIC "your_aws_iot_log_topic" // MQTT topic for publishing soil sensor logs
  #define AWS_IOT_SUBSCRIBE_TOPIC "your_aws_iot_log_topic" // MQTT topic for subscribing to soil sensor log responses
  #define AWS_IOT_SHADOW_UPDATE_DELTA "$aws/things/{your_thing}/shadow/update/delta" // MQTT topic for shadow updates with a delta between desired and reported states
  #define AWS_IOT_SHADOW_UPDATE "$aws/things/{your_thing}/shadow/update" // MQTT topic for all shadow updates
  #define AWS_IOT_SHADOW_ACCEPTED "$aws/things/{your_thing}/shadow/get/accepted" // MQTT topic for when a shadow get request is accepted
  #define AWS_IOT_SHADOW_REJECTED "$aws/things/{your_thing}/shadow/get/rejected" // MQTT topic for when a shadow get request is rejected
  #define AWS_IOT_SHADOW_GET "$aws/things/{your_thing}/shadow/get" // MQTT topic to get the device shadow
  
  // BLE UUIDs
  #define SERVICE_UUID        "your_service_uuid" // Unique identifier for the BLE service
  #define CHARACTERISTIC_UUID "your_characteristic_uuid" // Unique identifier for the BLE characteristic
  
  #define THINGNAME "your_thing_name" // Name associated with AWS IoT Thing
  #define catNum "your_device_catnumber" // The catalog number associated with the PlantPal device, which is physically located on the device
  
  extern const char AWS_IOT_ENDPOINT[]; // The endpoint associated with your AWS IoT account
  
  static const char AWS_CERT_CA[] PROGMEM = R"EOF(
  -----BEGIN CERTIFICATE-----
  {your_CA_certificate}
  -----END CERTIFICATE-----
  )EOF"; // AWS IoT CA Certificate 
  
  static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
  -----BEGIN CERTIFICATE-----
  {your_certificate}
  -----END CERTIFICATE-----
  )KEY"; // AWS IoT Certificate 
    
  // Device Private Key
  static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
  -----BEGIN RSA PRIVATE KEY-----
  {your_private_certificate}
  -----END RSA PRIVATE KEY-----
  )KEY"; // AWS IoT Private Certificate 
  
  extern byte aesKey[]; // 16 byte key for AES encryption
  extern byte aesIv[]; // 16 byte iv for AES encryption
  ```

  `utilities.cpp` **variables**:
  ```plain text
  const char AWS_IOT_ENDPOINT[] = "your_aws_endpoint"; // The endpoint associated with your AWS IoT account

  byte aesKey[] = { your_aesKey }; // 16 byte key for AES encryption
  byte aesIv[] = { your_aesIv }; // 16 byte iv for AES encryption
  ```

## Usage

1. Plug in and power the PlantPal device using a USB-C cable.
2. Use the following color code breakdown:
  - **White** = Initial setup/loading
  - **Orange** = Not connected to Wi-Fi
  - **Red** = Device is successfully connected to Wi-Fi but the plant requires water
  - **Green** = Device is successfully connected to Wi-Fi and the plant has sufficient water
  - **Blinking** = Indicates the device is actively thinking or working on a task (e.g., establishing a BLE/Wi-Fi connection). Pressing the button toggles connection states.
3. Use the PlantPal application to add the device to the system.
4. Use the PlantPal application to add Wi-Fi credentials.
5. Press the connection button on the PlantPal device to initiate the connection. Observe a blinking LED during this state.
6. The LED will change colors to indicate connectivity and the plant's water needs.
7. Ensure that the probe of the PlantPal device is inserted at least 2 inches into the plant's soil.
8. Detailed information about the plant's water needs can be observed using the PlantPal application dashboard.

## Documentation

This project uses Doxygen to generate documentation from the C++ code comments. 

## Future Features

We're excited about the upcoming features that will enhance the PlantPal experience:

1. **Stripe Checkout**: Streamline the purchasing process for PlantPal devices, making PlantPal commercially available to help people care for their plants.
2. **Notification System**: Implement text or push notifications to keep users updated on their plants' status, ensuring they never miss important alerts.
3. **Battery Management**: Introduce a battery indicator and management feature for PlantPal devices, helping users monitor power levels and extend device longevity.
4. **Variable Management**: Give user's the ability to select the type of plant the device is monitoring (low/high water plants) and adjust the sensitivity levels of the PlantPal device

## Contributing

If you want to contribute to this project, feel free to open an issue or submit a pull request. Any contributions, from bug fixes to new features, are welcome!

## License

This project is licensed under the MIT License. See the LICENSE file for more details.
