# Matter-environment-Sensor

# Table of Contents
- [Matter-environment-Sensor](#matter-environment-sensor)
- [Table of Contents](#table-of-contents)
  - [Description](#description)
  - [Installation](#installation)
  - [Version compatibility](#version-compatibility)
  - [Usage](#usage)

## Description
The environment sensor is a program to control 4 differents sensors. The device will be able to measure temperature, humidity, light and detect motion. The sensors are one SHT41, one VEML7700, and PIR sensor with an amplifier circuit. The device will communicate with other device with the Matter protocol.

## Installation
To install the project, you need to clone the repository and initialize the submodules. The project uses 3 differents libraries to control the sensors. The libraries are available as submodules in the components folder. To initialize the submodules, you need to run the following command:
```
git submodule update --init --recursive
```

The project uses the ESP-IDF framework. To install the framework, you need to follow the instructions available on the [Espressif website](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html). The project has been tested with the version 5.0 of the framework. Other version may not work with the project. You aslo need to install the ESP-Matter library. The library is available on the [Espressif GitHub](https://github.com/espressif/esp-matter). Follow the instructions available on the repository to install the library.

## Version compatibility
This project uses 3 different libraries to control the sensors. The libraries are available as submodules in the components folder. The library compatible with the project are:
- SHT41
  - Hydrogen1
- VEML7700 
  - Hydrogen1
- BusIO 
  - Hydrogen1

Other version of the libraries may not work with the project. They don't have been tested with the project. The project has been tested on ESP32-S3 and ESP32-H2 boards. The other board compatible with the framework should work with this project.

## Usage
