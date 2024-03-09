# Sensor System Monitoring

This repository contains a C program designed for monitoring and analyzing data from various sensors within a system. The program supports handling different sensor types, including tire sensors and power management units (PMUs), offering insights into their current statuses and performance metrics.

## Features

- **Multiple Sensor Support:** Handles data from tire sensors and power management units, each with unique metrics such as pressure, temperature, wear level, and power management details.
- **Dynamic Sensor Analysis:** Performs real-time analysis of sensor data to assess conditions such as tire pressure, temperature, wear level, and power management unit's energy usage and health.
- **Sensor Data Validation:** Automatically validates sensor data to ensure readings are within expected ranges. Invalid sensor data entries are identified and can be cleared from the system.
- **Modular Design:** Utilizes a structured approach with separate modules for defining sensor structs (`structs.h`), performing operations on sensor data (`operations.c`), and integrating all functionalities (`main.c`).

## Repository Structure

- `structs.h` - Defines the structures for different sensor types and their associated data.
- `operations.c` - Contains functions for analyzing and reporting on sensor data.
- `main.c` - The main program file that integrates sensor data reading, processing, and user interaction.
- `Makefile` - Used for compiling the program with make, simplifying the build process.

## Getting Started

1. **Compilation:** Use the provided Makefile to compile the program. Run `make` in the terminal within the project directory.
2. **Running the Program:** After compilation, run the program with `./sensor_monitoring`. The program expects sensor data input, which should be prepared in advance.
3. **Interacting with the Program:** The program supports commands for printing sensor information, analyzing sensor data, and clearing invalid sensor entries.
