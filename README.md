# MPU-6050 Driver

Author: Gonzalo Gabriel Fernandez

## Features
- Device initialization
- Sanity check
- Get Gyroscope and Accelerometer configuration word
- Full Scale selection for Gyroscope and Accelerometer
- Blocking read of raw Gyroscope, Accelerometer, and Temperature measurements
- Non-blocking read of raw Gyroscope, Accelerometer, and Temperature measurements

## Port
Currently, the microcontroller families supported are:
- STM32F103C8T6 (Blue Pill board) (STM32F1XX)
- STM32F429ZI (STM32F4XX)

The non-blocking read is supported via DMA.
